/*--------------------------------------------------------------------
  ModelPreviewTask.cpp

  Contains the implementation of the main application task.

  16/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "StdAfx.h"
#include "ModelPreviewTask.h"

using namespace UI;
using namespace Jet;

LOGCLIENT(ModelPreview);


/*-------------------------------------------------------------------
PROCEDURE: ModelPreviewTask
PURPOSE  : To construct the object
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
ModelPreviewTask::ModelPreviewTask():
Task("ModelPreviewTask"),
renderKernel(NULL),
model(NULL),
modelDB(NULL),
animationDB(NULL),
world(NULL),
camera(NULL),
viewPort(NULL),
ctrDialog(NULL), 
renderData(NULL),
done(false),
skeleton(NULL),
skeleVertex(NULL),
skeleVertexDB(NULL)
{

}

/*-------------------------------------------------------------------
PROCEDURE: ~ModelPreviewTask
PURPOSE  : To destroy the object
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
ModelPreviewTask::~ModelPreviewTask(){

  //Ensure the memory is de-allocated
  Done();
   
}


/*-------------------------------------------------------------------
PROCEDURE: Init
PURPOSE  : To initialise the task.
PRE : "os" is a valid operating system class
POST: The task has been initialised if it returns true.

NOTE: It is assumed that init. is only EVER called once for this object
      as resources from a successful call are not deallocated.
-------------------------------------------------------------------*/
bool ModelPreviewTask::Init(OS *os){

   // Setup the world.
   if(!CreateWorld(os)){
      LOGERR(("ModelPreviewTask::Init() Unable to create world"));
      return false;
   }

   //Setup the render data
   renderData = NewMem(RenderData());

   //Setup the main dialog
   ctrDialog = NewMem(ControlDialog()); 
   if(!ctrDialog->Init(renderKernel,this,renderData)){
      LOGERR(("ModelPreviewTask::Init() Unable to create dialog"));
      return false;
   }

   //Get the size of the window
   windowSize = renderKernel->GetDisplay()->GetSize();

   PString name;
   if(os->GetOptions()->Get("MeshName",name)){
      OpenInitialMesh(name);
   }


   //renderKernel->GetDisplay()->SetLocalAxesMode(true); 

   return Task::Init(os);
}

/*-------------------------------------------------------------------
PROCEDURE: OpenInitialMesh
PURPOSE  : To open a mesh passed from the OS.
PRE : ctrDialog has been set up successfully.
POST: If "name" is a valid filename and directory, the mesh is loaded
      and returns true. If "name" is a valid directory the directory 
      is loaded and returns true. Else returns false.
-------------------------------------------------------------------*/
bool ModelPreviewTask::OpenInitialMesh(PString name){
int i;
int len;
  
  len=name.Length();

  //Get the optional last character 
  if(name[len-1]=='/'||name[len-1]=='\\'){
    
    //Assume it is a directory and just set the directory 
    ctrDialog->SetCurrDirectory(name); 
    return true;
  }

  //Search the string for a "/" or "\"
  for(i=len-1;i>=0;i--){
    if(name[i]=='/'||name[i]=='\\'){
        break;
    }
  }

  //If we found a "/" or "\"
  if(i>-1){
    //Split the string
    PString pLeft=name;
    PString pRight=name;  
    pLeft.Left(i);
    pRight.Left(len);
    pRight.Right(len-i-1);

    //Set the directory 
    ctrDialog->SetCurrDirectory(pLeft); 
    

    //Since we cannot load a mesh before init. has finished calling..
    // set the variables to call on the first update
    initLoad=true;
    initPath=pLeft;
    initFilename=pRight;

    return true;
  }else{
    //It was not a valid string
    return false;
  }



}

/*-------------------------------------------------------------------
PROCEDURE: CloseTask
PURPOSE  : To tell the task to close on the next call to update.
           this task.
PRE : TRUE
POST: The task will now close on the next update
-------------------------------------------------------------------*/
void ModelPreviewTask::CloseTask(){
  done=true;
 return;
}
/*-------------------------------------------------------------------
PROCEDURE: Done
PURPOSE  : To complete any actions required upon the stopping of 
           this task.
PRE : TRUE
POST: All resources in use have been deallocated.
-------------------------------------------------------------------*/
void ModelPreviewTask::Done(){

   //NOTE:It appears that when you delete the renderkernel the scenegraph is deleted also
   // ALSO : The order of deletions is IMPORTANT!
   //NOTE: When the "world" is deleted, all children are deleted (I think)

   //Remove the main dialog
   if(ctrDialog ){ DeleteMemZ(ctrDialog );}

   //Remove the skeleton vertex 
   if(skeleVertex){
     DeleteMemZ(skeleVertex);
   }
   if(skeleVertexDB){
     DBManager.RemoveReference(skeleVertexDB);
   }

   //Remove the skeleton (it is an attached particle structure)???
   //if(skeleton) {DeleteMemZ(skeleton);}
   
   //Remove the model DB
   if(modelDB){
     DBManager.RemoveReference(modelDB); 
   }

   //Remove the animation DB
   if(animationDB){
     DBManager.RemoveReference(animationDB); 
   }

   //Remove the model??
   //if(model){ DeleteMemZ(model);}

   //Remove camera and viewport
   if(viewPort  ){ DeleteMemZ(viewPort  );}
   if(camera    ){ DeleteMemZ(camera    );}

   //Remove renderkernel
   if(renderKernel){ DeleteMemZ(renderKernel);}
   
   //Remove the render data
   if(renderData){ DeleteMemZ(renderData);}

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: Update
PURPOSE  : To update this task with the current time.
PRE : Init has been called first. "dt" contains the amount of time 
      since the last call.
POST: Returns CONTINUE if this task is to be updated again.
              STOP if this task does no longer wish to be updated.
              EXIT to stop the taskmanager it is assigned to.
-------------------------------------------------------------------*/
Task::UpdateCode ModelPreviewTask::Update(float dt){



  //Load the initial mesh if it exists
  if(initLoad){
    LoadMesh(initPath,initFilename);
    ctrDialog->UpdateModelTab(); 
    initLoad=false;
  }

  //Code to rotate orbit the model 
  //  (probably not the best place to put this and rotation speed may 
  //   vary on different computers)
  if(model&&renderData->orbit){
    Matrix3 mat;
    mat.Identity(); 
    mat.RotateZ(dt/-2.0f);
    model->Rotate(mat); 
  }

  //Update the selected vertex orientation 
  //(probably not the best postion as an animation observer would be better)
  if(skeleVertex && skeleVertex->IsVertexSelected()){

    //Get the current orientation
    Vector3 orien(skeleVertex->GetVertexOrientation());
    renderData->vertexRoll  = (int)((float)orien.x * (180.0f/PI));
    renderData->vertexPitch = (int)((float)orien.y * (180.0f/PI));
    renderData->vertexYaw   = (int)((float)orien.z * (180.0f/PI));

      
    // Update : This breaks the model/interface seperation
    ctrDialog->UpdateBoneVertexTab(); 
  }

  
  //Only continue if are not finished yet
  if(done){
     return Task::EXIT;
  }else{
     return Task::CONTINUE;
  }
}


/*-------------------------------------------------------------------
PROCEDURE: CreateWorld
PURPOSE  : To create the visual world.  
PRE : "os" is a valid operating system
POST: Returns true if the world was init. properly, false if not.
-------------------------------------------------------------------*/
bool ModelPreviewTask::CreateWorld(OS *os){

   //Init the render system
   renderKernel = NewMem(RenderKernel);
   if(!renderKernel->Init(os)){
      DeleteMemZ(renderKernel);
      return false;
   }

   //Create a virtual scene. 
   world = NewMem(ParticleStructure);
   renderKernel->GetSceneGraph()->AddScene(world);

   //Setup the camera
   camera = NewMem(Camera);
   camera->SetPosition(Vector3(0.0f, -5.0f, 0.0f));
   camera->AlignToTarget(Vector3(0.0f, 0.0f, 0.0f));

   //Setup the camera to look at the world 
   camera->SetScene(world);

   // Before we can render anything we need to set up a viewport
   // that defines the area of the client region where we want
   // something to be drawn. Default view rect is the full client
   // area. This viewport is automatically registered with the
   // layer manager.
   viewPort = NewMem(ViewPort(camera,Rect(0,0,renderKernel->GetDisplay()->GetSize())));

 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadMesh
PURPOSE  : To load a mesh into the scene.
PRE : Init has been called successully.
POST: Returns true if the mesh was loaded, false if not.
-------------------------------------------------------------------*/
bool ModelPreviewTask::LoadMesh(PString path,PString name){
  //LOGERR(("Path: %s name:%s", (const char *)path  ,(const char *)name));
 
  //Create a new object
  ModelObject *newObject = NewMem(ModelObject(renderKernel));
  
  //Create a resource to load the model from
  ResourceDatabase *newModelDB = DBManager.LoadDatabase(path);

  //Try to Load the mesh
  if(!newObject->LoadMesh(newModelDB,name)){
      DBManager.RemoveReference(newModelDB);
      DeleteMemZ(newObject);
      return false;
  }

  //Unload previous mesh,mesh DB and animation DB
  if(model){
    world->Remove(model);
    DeleteMemZ(model);
    DBManager.RemoveReference(modelDB); 

    //Unload previous animation
    if(animationDB){
      DBManager.RemoveReference(animationDB);
    }

  }

  //Assign over the variables
  model = newObject;
  modelDB = newModelDB;

  //Init the skeleton,bone vertex and display data
  SetSkeleton(renderData->skeleton);
  SetBoneVertex(renderData->showVertex); 
  SetModelVisiable(renderData->modelDisplayed);
  SetTransparency(renderData->transparency);

  //Load the mesh model and assign the pointers
  world->Add(newObject);
 
  //TODO : Find why loading the skeleton here, causes a crash

  //Centre model
  model->CentreModel(); 

  //Allign the camera
  AllignCamera();

  //Get/Set data on the model
  renderData->currModelName = name;
  renderData->totPoly =model->GetMaxTriangles();
  renderData->currPoly=model->GetNumTriangles(); 
  renderData->currDetailPercent = 100; 
  renderData->orbit = false; 
  renderData->scaleDetail=model->IsDetailScaleable();

  //Set the data on the animation
  renderData->currAnimationName="None";
  renderData->speed=100;
  renderData->reverse=false;

 return true;
}



/*-------------------------------------------------------------------
PROCEDURE: MoveMesh
PURPOSE  : To move a mesh that is in the scene.
PRE : Init has been called successfully.
POST: Returns true if the mesh was moved, false if not.
-------------------------------------------------------------------*/
bool ModelPreviewTask::MoveMesh(Vector2 v){
Matrix3 mat;
  
  //Check for an exitsing model
  if(!model){
     return false;
  }

  //Init. our matrix to be sure
  mat.Identity();

  //If we are performing a rotation, adjust the variables
  if(renderData->movement>=ROTATE_ALL && 
     renderData->movement<=ROTATE_Z){
     v.x=v.x/250.0f;
     v.y=v.y/250.0f;

  }

  //Find the rotation movement state
  switch(renderData->movement){
    case(ROTATE_ALL) : mat.RotateX(v.y);
                       mat.RotateZ(v.x);
                       model->Rotate(mat);
                       break;

    case(ROTATE_X  ) : mat.RotateX(v.y);
                       model->Rotate(mat);        
                       break;

    case(ROTATE_Y  ) : mat.RotateY(v.x);
                       model->Rotate(mat);        
                       break;
    
    case(ROTATE_Z  ) : mat.RotateZ(v.x);
                       model->Rotate(mat);        
                       break;
  }

 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadAnimation
PURPOSE  : To load a animation into the scene.
PRE : Init has been called successfully.
POST: Returns true if the animation was loaded, false if not.
-------------------------------------------------------------------*/
bool ModelPreviewTask::LoadAnimation(PString path,PString name){

  //Check for an exitsing mesh
  if(!model){
    return false;
  }

  //Create a resource to load the animation from
  ResourceDatabase *newAnimationDB = DBManager.LoadDatabase(path);


  //See if the user has selected a bone
  if(skeleVertex && skeleVertex->IsVertexSelected()){ 

    //Try to Load the animation
    if(!model->LoadAnimation(newAnimationDB,name,skeleVertex->GetVertexName())){
        DBManager.RemoveReference(newAnimationDB);
        return false;
    }
  }else{

    //Try to Load the animation
    if(!model->LoadAnimation(newAnimationDB,name)){
        DBManager.RemoveReference(newAnimationDB);
        return false;
    }
  }

  //Unload previous animationDB
  if(animationDB){
    DBManager.RemoveReference(animationDB);
  }

  //Load the animation DB
  animationDB = newAnimationDB;
 
  //Get data on the animation
  renderData->currAnimationName=name;
  renderData->speed=100;
  renderData->reverse=false;


 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: AllignCamera
PURPOSE  : To allign the camera so that the object fits within the 
           scene.
PRE : Init has been called successfully.
POST: The object now fits within the scene.
-------------------------------------------------------------------*/
void ModelPreviewTask::AllignCamera(){
double currDist,maxDist;
double maxAngle;
float cameraPos;

  //If we have no model we cannot do anything
  if(!model){
    return;
  }

  //Get the greatest distance from the centre to a point
  maxDist = model->GetXLimits().y-model->GetXLimits().x;

  currDist= model->GetYLimits().y-model->GetYLimits().x;
  if(maxDist<currDist){
    maxDist=currDist;
  }

  currDist= model->GetZLimits().y-model->GetZLimits().x;
  if(maxDist<currDist){
    maxDist=currDist;
  }
  //maxDist=maxDist/2;

  //Lets be careful and add an extra 50%
  //maxDist+=0.5*maxDist;

  //Find the max. angle of the camera
  maxAngle=camera->GetHorizontalFOV();   
  if(maxAngle<camera->GetVerticalFOV()){
    maxAngle=camera->GetVerticalFOV();   
  }
  maxAngle=maxAngle/2;

  //Ensure we can tan the angle
  if(maxAngle*180/PI > 89.5f){
    cameraPos=0.0;
  }else{

    //Calculate the camera position (check for 0 divide) 
    maxAngle=tan(maxAngle);
    if(maxAngle>0.0){
      cameraPos=(float)(maxDist/maxAngle);
    }else{
      cameraPos=1000.0f;
    }
  }

  //Update the camera position (should already be looking at (0,0,0) but set it anyway)
  camera->SetPosition(Vector3(0.0f,-cameraPos,0.0f)); 
  camera->AlignToTarget(Vector3(0.0f,0.0f, 0.0f));
 
  renderData->cameraDist=cameraPos; 
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetDetail
PURPOSE  : To set the detail level of the object.
PRE : Init has been called successfully.
POST: If it is possible to adjust the detail of the model, it is 
      done.
-------------------------------------------------------------------*/
void ModelPreviewTask::SetDetail(int detail){

 //Set the detail and get the new number of triangles
 if(model){
   model->SetDetail((float)detail/100.0f); 
   renderData->currPoly=model->GetNumTriangles();  
 }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetAnimationSpeed
PURPOSE  : To set the speed of the animation.
PRE : Init has been called successfully.
POST: If an animation and model is loaded, it's speed is changed.
-------------------------------------------------------------------*/
void ModelPreviewTask::SetAnimationSpeed(int speed){

 //Set the speed if we have a model
 if(model){
   model->SetAnimationSpeed((float)speed/100.0f); 
   renderData->speed = speed; 
 }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetAnimationBackwards
PURPOSE  : To set the animation to play backwards.
PRE : Init has been called successfully.
POST: If an animation and model is loaded, it's will play backwards 
      if bck =true.
-------------------------------------------------------------------*/
void ModelPreviewTask::SetAnimationBackwards(bool bck){

 //Set the speed if we have a model
 if(model){
   model->SetAnimationBackwords(bck); 
   renderData->reverse = bck; 
 }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetSkeleton
PURPOSE  : To set the displaying of the skeleton.
PRE : Init has been called successfully.
POST: A skeleton is created so that when a model is loaded it will 
      be used. 
-------------------------------------------------------------------*/
void ModelPreviewTask::SetSkeleton(bool skele){

  //Test if we are creating a skeleton or not
  if(skele){ 
    //If we do not currently have a skeleton create one
    if(!skeleton){
       skeleton = NewMem(SkeletonDisplay());
       
       Color c(1.0f,1.0f,1.0f,1.0f);
       skeleton->SetSkeletonColour(c,renderKernel->GetDisplay()); 
    }
    //If we currenly have a model, init the skeleton
    if(model){
      skeleton->LoadParticleStructure(model); 

      //Ensure we do not add the skeleton twice
      world->Remove(skeleton); 
      world->Add(skeleton); 
    }
  }else{
     //Turn off the skeleton
     if(skeleton){ 
         world->Remove(skeleton); 
         DeleteMemZ(skeleton);
     }
  }

  renderData->skeleton =skele;

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetBoneVertex
PURPOSE  : To set the displaying of the bone vertex.
PRE : Init has been called successfully.
POST: If true a bone vertex is created so that when a model is loaded
      it will be used. If false, an existing bone vertex will be 
      turned off.
-------------------------------------------------------------------*/
void ModelPreviewTask::SetBoneVertex(bool bv){

  //Test if we are creating a bone vertex or not
  if(bv){
      
    //If we do not currently have a bone vertex, create one
    if(!skeleVertex){
       skeleVertex = NewMem(SkeletonVertex());

       if(skeleVertexDB){
          DBManager.RemoveReference(skeleVertexDB);
       }
       skeleVertexDB=DBManager.LoadDatabase("Data",""); 

    }
    //If we currenly have a model, init the bone vertex
    if(model){
       skeleVertex->LoadParticleStructure(renderKernel,model,skeleVertexDB,"ball.im");  
       skeleVertex->SetScale(0.1f); 
    }
  }else{
     //Turn off the skeleton
     if(skeleVertex){ 
         skeleVertex->RemoveVertexMeshes(); 
         DeleteMemZ(skeleVertex);
         DBManager.RemoveReference(skeleVertexDB);
         skeleVertexDB=NULL;

         //Reset selection data
         renderData->selectVertex=false;
         renderData->vertexName="None";
         renderData->vertexRoll=0;
         renderData->vertexPitch=0; 
         renderData->vertexYaw=0; 
     }
  }

  //Assign render data show vertex
  renderData->showVertex=bv; 
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetWireframe
PURPOSE  : To set the displaying in wireframe mode.
PRE : Init has been called successfully.
POST: If wire=true wireframe mode is enabled, else wireframe mode is
      disabled.
-------------------------------------------------------------------*/
void ModelPreviewTask::SetWireframe(bool wire){
 
  //Set the render data   
  renderData->wireframe=wire;  

  //Set the wireframe mode
  renderKernel->GetDisplay()->SetWireFrameMode(wire); 

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetLocalAxis
PURPOSE  : To set the displaying in local axis.
PRE : Init has been called successfully.
POST: If ls=true, local axis mode is enabled, else local axis mode is
      disabled.
-------------------------------------------------------------------*/
void ModelPreviewTask::SetLocalAxis(bool la){

  //Set the render data   
  renderData->localaxis=la;  

  //Set the wireframe mode
  renderKernel->GetDisplay()->SetLocalAxesMode(la); 

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetModelVisiable
PURPOSE  : To set if the model is visiable or not.
PRE : Init has been called successfully.
POST: If vis=true, the model is visiable, else the model is hidden.
-------------------------------------------------------------------*/
void ModelPreviewTask::SetModelVisiable(bool vis){

   //Only proceed if we have a model
   if(model){
     renderData->modelDisplayed=vis;
   
     //Opacity is used instead of Hide/Show as it still anable 
     //   animation updates
     if(vis){
       model->SetOpacity((float)renderData->transparency/100.0f);
     }else{
       model->SetOpacity(0.0f);   
     }
   }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetTransparency
PURPOSE  : To set the model's transparency.(1-100) in percent
PRE : Init has been called successfully.
POST: If a model is loaded, it's transparency is set to "trans".
-------------------------------------------------------------------*/
void ModelPreviewTask::SetTransparency(int trans){

  //Check for model and set transparency
  if(model && renderData->modelDisplayed){
    model->SetOpacity((float)trans/100.0f);
    renderData->transparency=trans; 
  }

 return;
}


/*-------------------------------------------------------------------
PROCEDURE: SetCameraDistance
PURPOSE  : To set the distance between the model and the object.
PRE : Init has been called successfully.
POST: The camera's distance has been set.
-------------------------------------------------------------------*/
void ModelPreviewTask::SetCameraDistance(float dist){

  renderData->cameraDist=dist; 

  //Set the distance then re-allign in-case of negative distance
  camera->SetPosition(Vector3(0.0f,-dist,0.0f)); 
  camera->AlignToTarget(Vector3(0.0f,0.0f, 0.0f));
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SelectVertexMesh
PURPOSE  : To select a vertex mesh based on screen coordinates.
PRE : Init has been called successfully.
POST: Returns true if a selection was made.
-------------------------------------------------------------------*/
bool ModelPreviewTask::SelectVertexMesh(int x,int y){
bool retVal;

  //If we do not have a skeleton vertex we cannot proceed   
  if(!skeleVertex){
    return false;      
  }

  //Create a line through the viewport at the current position
  Line testLine = camera->GetLine((float)x/windowSize.w, (float)y/windowSize.h);

  //Perform the selection
  retVal=skeleVertex->TestCollision(testLine,camera->GetPosition());
  
  //Retrieve selection data
  if(retVal){
    renderData->selectVertex=true; 
    renderData->vertexName=skeleVertex->GetVertexName(); 
    Vector3 orien(skeleVertex->GetVertexOrientation());
    renderData->vertexRoll  = (int)((float)orien.x * (180.0f/PI));
    renderData->vertexPitch = (int)((float)orien.y * (180.0f/PI));
    renderData->vertexYaw   = (int)((float)orien.z * (180.0f/PI));
  }else{
    renderData->selectVertex=false; 
    renderData->vertexName=skeleVertex->GetVertexName(); 
    renderData->vertexRoll  = 0;
    renderData->vertexPitch = 0;
    renderData->vertexYaw   = 0;
  }


  return retVal;
}


/*-------------------------------------------------------------------
PROCEDURE: SetVertexOrientation
PURPOSE  : To set the orientataion of the selected bone vertex.
PRE : Init has been called successfully.
POST: Returns true if there exists a selected bone veretx and it has 
      it's orientation moved.
-------------------------------------------------------------------*/
bool ModelPreviewTask::SetVertexOrientation( int vRoll,int vPitch,int vYaw){

  //If we don't have a vertyex bones displayed
  if(!skeleVertex){
    return false;
  }

  //If we have no vertex to select  
  if(!skeleVertex->IsVertexSelected()){
    return false;        
  }

  //Create new orientations in radians
  Vector3 newOrien((float)vRoll  * (PI/180.0f),
                   (float)vPitch * (PI/180.0f),
                   (float)vYaw   * (PI/180.0f));


  //Set the orientation
  skeleVertex->SetVertexOrientation(newOrien); 

 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: SaveOrientation
PURPOSE  : To save the orientation (TEST METHOD)
PRE : Init has been called successfully.
POST: The orientation has been saved.
-------------------------------------------------------------------*/
void ModelPreviewTask::SaveOrientation(){

  if(!model){
     return;
  }

  //Save the data
  SkeletonFile::SaveSkeleton(model,"test.ske",SKELE_BOTH /*SKELE_ORIENTATION*/);
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadOrientation
PURPOSE  : To load the orientation (TEST METHOD)
PRE : Init has been called successfully.
POST: The orientation has been loaded.
-------------------------------------------------------------------*/
void ModelPreviewTask::LoadOrientation(){

  if(!model){
     return;
  }

  //Load the data
  SkeletonFile::LoadSkeleton(model,"test.ske");

 return;
}
