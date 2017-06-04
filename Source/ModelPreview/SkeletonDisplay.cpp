/*--------------------------------------------------------------------
  SkeletonDisplay.cpp

  Contains the implementation of the class that allows the 
  displaying of a ParticleStructure Skeletion

  09/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "StdAfx.h"
#include "SkeletonDisplay.h"

using namespace Jet;

/*-------------------------------------------------------------------
PROCEDURE: SkeletonDisplay
PURPOSE  : To construct the object
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
SkeletonDisplay::SkeletonDisplay():
autoUpdate(true),
colour(1.0f,1.0f,1.0f),
numBones(0),
display(NULL),
pStruct(NULL),
pvArray(NULL)
{

}

/*-------------------------------------------------------------------
PROCEDURE: ~SkeletonDisplay
PURPOSE  : To destruct the object
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
SkeletonDisplay::~SkeletonDisplay(){

  //Just unload particle structure data
  UnLoadParticleStructure();
}


/*-------------------------------------------------------------------
PROCEDURE: LoadParticleStructure
PURPOSE  : To Load a particle structure's bone data. 
PRE : ps points to a valid ParticleStructure. (see below)
POST: The bone data in the particle structure has been loaded and is 
      ready for rendering. If a previous particle structure was 
      loaded, it is unloaded.

NOTE: If only ONE bone is found, the particle structure will not load 
      it. (Cannot draw a line with only 1 bone)

      It is also important to note that the particle structure MUST 
      have a finite beginning and end or this function may consume all 
      available memory then crash.
-------------------------------------------------------------------*/
unsigned int SkeletonDisplay::LoadParticleStructure(ParticleStructure *ps){

  //Free pre-existing data
  UnLoadParticleStructure();

  //Set the particle structure
  pStruct=ps;

  //Get the particles that are bones
  GetBones(ps);

  //Set the number of bones
  numBones= pArray.Count();

  //Allocate render array 
  if(numBones>0){
    pvArray=NewArray(PackedVertex[numBones]);
  }

  //Update the render Array
  UpdateSkeleton();

  //Reset the bounding volume for the new particle structure
  ResetBoundingVolume();
 return (numBones/2)+1;
}

/*-------------------------------------------------------------------
PROCEDURE: UnLoadParticleStructure
PURPOSE  : To free all memory associated with a previous 
           ParticleStructue loading. However this can be called at 
           any time.
PRE : TRUE
POST: All data previously associated with a ParticleStructure has 
      been removed.
-------------------------------------------------------------------*/
void SkeletonDisplay::UnLoadParticleStructure(){

  //Free Pointers to Particles
  pArray.RemoveAll();

  //Free memory for the render Array
  if(pvArray){
      DeleteArrayZ(pvArray);
  }  
  
  //Set number of bones to zero
  numBones=0;

  //Re-set particle structure
  pStruct=NULL;

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetSkeletonColour
PURPOSE  : To set the colour that the bone lines will be rendered in.
           If you have not called SetDisplay, you must pass the 
           display pointer when calling or the colour will not 
           change.
PRE : TRUE
POST: The colour has been assigned to colour "c"
-------------------------------------------------------------------*/
void SkeletonDisplay::SetSkeletonColour(Color &c,Display *d){
unsigned int i;

  //Just copy colour data
  colour=c;

  //If we do not have a display pointer we cannot do anything 
  //   so return
  if(display==NULL&&d==NULL){
    return;
  }
  if(display!=NULL&&d==NULL){
    d=display;
  }

  //Create the display colour
  displayColour=d->ConvertColor(colour);

  //Loop an assign the colour to each bone
  for(i=0;i<numBones;i++){
    pvArray[i].color=displayColour; 
  }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: GetNumBones
PURPOSE  : To get the number of bones that have been loaded in from 
           the current ParticleStructure. It is not the number of REAL 
           bones (ie as in a start and an end point) but the number 
           of points that move vertex data.
PRE : TRUE
POST: The number of bones (if any) are returned
-------------------------------------------------------------------*/
unsigned int SkeletonDisplay::GetNumBones()const{
 
 return (numBones/2)+1;
}

/*-------------------------------------------------------------------
PROCEDURE: SetDisplay
PURPOSE  : To set the display to be used when rendering the 
           skeleton. This is not necessary to call to render as it 
           exists only to set display specific variables (ie colour).
PRE : TRUE
POST: The number of bones (if any) are returned
-------------------------------------------------------------------*/
void SkeletonDisplay::SetDisplay(Display *d){
  //Set the display pointer
  display=d;
 return;
}


/*-------------------------------------------------------------------
PROCEDURE: SetAutoUpdate
PURPOSE  : To set wheither or not the bone position data is updated 
           on each render call. (By default = true)
PRE : TRUE
POST: The auto update facility has been set to "update"
-------------------------------------------------------------------*/
void SkeletonDisplay::SetAutoUpdate(bool update){
   autoUpdate=update; 
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: UpdateSkeleton
PURPOSE  : To update the skeleton with the latest positions of the 
           bones.
PRE : TRUE
POST: The skeleton data has been updated
-------------------------------------------------------------------*/
void SkeletonDisplay::UpdateSkeleton(){
unsigned int i;
Vector3  v;

  //Test for array being NULL
  if(pvArray!=NULL){

     //Move the bounding volume area
     if(pStruct!=NULL){     
        boundingVolume->SetCenter(pStruct->GetPosition());
     }

    //Loop for all bones  
    for(i=0;i<numBones;i++){
       //v=pArray[i]->GetPosition();
       v=Vector(0.0f,0.0f,0.0f);
       pArray[i]->LocalToGlobal(v);
       pvArray[i].SetPosition(v); 
       //pvArray[i].color=displayColour;  
    }
  }

 return;
}
    
/*-------------------------------------------------------------------
PROCEDURE: ResetBoundingVolume
PURPOSE  : To resize the bounding volume to encompass all of the 
           object.
PRE : TRUE
POST: The bounding volume has been resized.

NOTE: Not currently working properly.
-------------------------------------------------------------------*/
void JETAPI SkeletonDisplay::ResetBoundingVolume(){
  // Check our children to make sure they are covered by the bounding volume
  ParticleStructure::ResetBoundingVolume();

  // Make sure that the bounding volume has some size
  if (boundingVolume){

      //Re-Copy data from the particle structure
     if(pStruct!=NULL){     
        //Get the bounding volume for the particle structure
        BoundingVolume *b=pStruct->GetBoundingVolume();

        //Reassing all the data
        boundingVolume->SetCenter(pStruct->GetPosition());
        boundingVolume->SetRadius(b->GetRadius());
        boundingVolume->SetRadiusXY(b->GetRadiusXY());
     }
     
  }else{
      // First time so create a bounding volume
      if(pStruct!=NULL){     
         //Just create a copy of the particle structure
         pStruct->ResetBoundingVolume();
         SetBoundingVolume(pStruct->GetBoundingVolume()->Clone());
         boundingVolume->SetCenter(pStruct->GetPosition());
      }else{
         //Unsure weither a reset wheither a reset is suppose to ensure 
         //  the creation of a bounding volume so to be safe...
         BoundingVolume *sphere = NewMem(BoundingVolume(GetPosition(), 10.0f));
         sphere->SetRadiusXY(10.0f);
         SetBoundingVolume(sphere);
      }
      
  }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: Cull
PURPOSE  : To add this object to the visiable list in the view???
PRE : TRUE
POST: If visiable, the object has been added to the view's visiable 
      list.
-------------------------------------------------------------------*/
void JETAPI SkeletonDisplay::Cull(ViewPort &view, const Matrix4 &trans){

  //Call Base class Cull first to manage the children
  ParticleStructure::Cull(view,trans);

  // Check the visible flag
  if (visible){

    // Add our object to the visible list in the viewport
    view.AddToVisibleList(VisibleElement(trans, this));

  // Remember that the culled flag is already cleared in our update routine
  }else{
    culled = true;
  }
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: Render
PURPOSE  : To render the current scene to the display area.
PRE : TRUE
POST: If a particle structure has been loaded, the skeleton has been 
      drawn. However it may not be the latest position as it depends 
      if auto-update is enabled or when UpdateSkeleton was last called.
-------------------------------------------------------------------*/
void JETAPI SkeletonDisplay::Render(Display *display, ViewPort &viewport, Matrix4 &transformation){
//bool depthTest;
bool texMode;

  //Check for auto-update
  if(autoUpdate){
     UpdateSkeleton();
  }
  
  //Test array for null
  if(pvArray!=NULL){
    //Load the transformation matrix
    display->LoadMatrix(transformation);

    //Get the current depth test mode 
    //depthTest=display->GetDepthTestMode();

    //Disable depth testing 
    //display->SetDepthTestMode(false);

    //Disable textures
    texMode = display->GetTextureMode(); 
    display->SetTextureMode(false);

    //Do the render
    display->DrawArray(D_LINES,pvArray,numBones); 

    //Re-set textures
    display->SetTextureMode(texMode);

    //Re-set depthtest
    //display->SetDepthTestMode(depthTest);
  }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: GetBones
PURPOSE  : To get the bones in a particle structure. Assumes bone's 
           name start with "b.". This method currently uses recursion 
           so it may be slow. The algromithm is depth first traversial.
PRE : The particle passed is valid.
POST: The particle array "pArray" has been populated with all 
      particles that are bones.
-------------------------------------------------------------------*/
void SkeletonDisplay::GetBones(Particle *p,Particle *lastBone) {
unsigned int i; 
unsigned int numChild;
    
   //Get the first 2 characters of this bone's name
   PString pname=p->GetName();
   pname.Left(2);
     
   //If this particle is a bone,  
   if(pname.Compare("b.")==0){

      //If we have a parent bone
      if(lastBone!=NULL){
          
          //Add the parent and the child to the array
          pArray.Add(lastBone);
          pArray.Add(p);
      }

      //Set this particle as the parent bone
      lastBone=p;
   }
   
    
   //Call on this particles children 
   numChild=p->GetNumChildren();
   for(i=0;i<numChild;i++){
       //Recursive call
       GetBones(p->GetChild(i),lastBone);
   }


 return;
}
