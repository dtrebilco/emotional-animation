/*--------------------------------------------------------------------
  ModelObject.cpp

  Contains the implementation of the class that loads and maintains an 
  object within a scene.

  17/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "StdAfx.h"
#include "ModelObject.h"

using namespace Jet;

LOGCLIENT(ModelObject);

/*-------------------------------------------------------------------
PROCEDURE: ModelObject
PURPOSE  : To construct the object
PRE : "rk" is a valid render kernel.
POST: The object has been constructed.
-------------------------------------------------------------------*/
ModelObject::ModelObject(RenderKernel *rk):
ParticleStructure(),
mesh(NULL), 
object(NULL),
skeleton(NULL),
animator(NULL),
renderKernel(rk),
testEmotion(NULL),
xLimits(0.0f,0.0f),
yLimits(0.0f,0.0f),
zLimits(0.0f,0.0f)
{

}

/*-------------------------------------------------------------------
PROCEDURE: ~ModelObject
PURPOSE  : To destroy the object
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/

ModelObject::~ModelObject(){
  //Assume that since we inherit from particle structure that 
  // it will destroy all children (ie mesh is destroyed)
/*
  if(mesh){
      Remove(mesh);
      DeleteMemZ(mesh);
  }
*/
  if(animator   ){ DeleteMemZ(animator);}
  if(testEmotion){ DeleteMemZ(testEmotion);}
  if(skeleton   ){ DeleteMemZ(skeleton);}


}



/*-------------------------------------------------------------------
PROCEDURE: LoadMesh
PURPOSE  : To load a mesh from a database.
PRE : TRUE
POST: If a mesh could be loaded returns true
      else returns false
-------------------------------------------------------------------*/
bool ModelObject::LoadMesh(ResourceDatabase *database,const PString &name){

  //Init. the geometry loader with the database
  GeometryBaseInstance geometry;
  geometry.Init(database, name);
/*
  //If we cannot load the file
  if(!geometry.IsAvailable()){
      return false;
  }
*/

  //Attempt to load in the mesh
  ParticleStructure *ps = geometry.CreateInstance(renderKernel, this);

  //If we did not load anything
  if(!ps){
      return false;
  }

  //If we have a previously loaded mesh remove it
  if(mesh){
    this->Remove(mesh);
    DeleteMemZ(mesh);
    object=NULL;
  }

  //If it was a mesh that was loaded, we can get a mesh pointer to it.
  mesh = dynamic_cast<IndexedMeshInstance *>(ps);
  //If it is not a mesh, the load failed
  if (!mesh){
    this->Remove(ps);
    DeleteMem(ps);
    return false;
  }

  //If it was a scaleable mesh (possibly) we may be able to get a pointer to it.
  object = dynamic_cast<ScalableDetail*>(ps);
  if(object){
     object->SetDetailLocked(true);
  }

  //Find the x,y,z limits of this mesh
  FindLimits();

  //Remove a previous animation and skeleton
  if(testEmotion){DeleteMemZ(testEmotion);}
  if(animator){DeleteMemZ(animator);}
  if(skeleton){DeleteMemZ(skeleton);}
  
 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadAnimation
PURPOSE  : To load a animation from a database. If a bone is given, 
           the animation will try to be loaded onto that bone.
PRE : TRUE
POST: If a animation could be loaded returns true
      else returns false
-------------------------------------------------------------------*/
bool ModelObject::LoadAnimation(ResourceDatabase *database,const PString &name,const PString boneName){


  //Create a skeleton for the given object
  if(!skeleton){
      skeleton = NewMem(EmotionSkeletonModifier(this, renderKernel));
  } 

  //Add an animation interface
  if(!animator){
    animator = NewMem(Animator(database, skeleton));
  }

  //Try to load onto the bone
  if(boneName!=PString("")){
    
    //Try to play the base animation
    if(animator->Add(NULL, name, boneName, AnimationModifier::CYCLIC | AnimationModifier::ROOT_HORIZ_OFF,
                   1.0f, 0.0f, 1.0f)!=OK){
      return false;
    }

  }else{
    //Load the animation onto the root bone (and remove previous animations)
    animator->RemoveAll();

    //Try to play the base animation
    if(animator->Add(NULL, name, AnimationModifier::CYCLIC | AnimationModifier::ROOT_HORIZ_OFF,
                     1.0f, 0.0f, 1.0f)!=OK){
      return false;
    }
  }

  //Testing emotion
  if(!testEmotion){
     testEmotion=NewMem(EmotionModifier(skeleton));
  }

  //testEmotion->LoadModifier(MOD_POSE,1.00f,"Emotions","Happy","Human"); 
  //testEmotion->LoadModifier(MOD_AF  ,1.00f,"Emotions","Happy","Human"); 
  //testEmotion->LoadModifier(MOD_RAND,1.00f,"Emotions","Angry","Human"); 
  testEmotion->LoadModifier(MOD_GRAV,0.40f,"Emotions","Depressed","Human"); 

 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: GetNumTriangles
PURPOSE  : To get the number of triangles in this model.
PRE : TRUE
POST: The number of triangles is returned.
-------------------------------------------------------------------*/
int ModelObject::GetNumTriangles() const {
  //First check that we have an object 
  if (object){
    return object->GetNumTriangles();
  }else{
      if(mesh){
         return mesh->GetMaxTriangles();          
      }else{
        return 0;
      }
  }
}


/*-------------------------------------------------------------------
PROCEDURE: GetMaxTriangles
PURPOSE  : To get the maximum number of triangles in this model.
PRE : TRUE
POST: The maximum number of triangles is returned.
-------------------------------------------------------------------*/
int ModelObject::GetMaxTriangles() const {
  
  //First check that we have an object 
  if (object){
    return mesh->GetMaxTriangles();
  }else{
    return 0;
  }
}


/*-------------------------------------------------------------------
PROCEDURE: SetDetail
PURPOSE  : To set the detail of this object.
PRE : TRUE
POST: If possible, the detail is set to "detail"
-------------------------------------------------------------------*/
void ModelObject::SetDetail(float detail){
  if (object){
    object->SetDetail(detail);
  }
 return;
}


/*-------------------------------------------------------------------
PROCEDURE: SetScale
PURPOSE  : To set the scale of this object.
PRE : TRUE
POST: If possible, the model is scaled to "scale".
-------------------------------------------------------------------*/
void ModelObject::SetScale(float scale){
  if (mesh){
    mesh->SetScale(Vector3(scale,scale,scale));
  }
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: GetXLimits
PURPOSE  : To return the maximum and minimium of the x direction in 
           this mesh.The order in the vector is (min,max) 
PRE : TRUE
POST: The limits are returned
-------------------------------------------------------------------*/
Vector2 ModelObject::GetXLimits(){

 return xLimits;
}

/*-------------------------------------------------------------------
PROCEDURE: GetYLimits
PURPOSE  : To return the maximum and minimium of the y direction in 
           this mesh.The order in the vector is (min,max) 
PRE : TRUE
POST: The limits are returned
-------------------------------------------------------------------*/
Vector2 ModelObject::GetYLimits(){

 return yLimits;
}

/*-------------------------------------------------------------------
PROCEDURE: GetZLimits
PURPOSE  : To return the maximum and minimium of the z direction in 
           this mesh. The order in the vector is (min,max)
PRE : TRUE
POST: The limits are returned
-------------------------------------------------------------------*/
Vector2 ModelObject::GetZLimits(){

 return zLimits;
}

/*-------------------------------------------------------------------
PROCEDURE: FindLimits
PURPOSE  : To calculate the x,y,z limits of this object. 
           (ie the bounding area)
PRE : TRUE
POST: If a model is loaded, the limits of it have been calculated.

NOTE: This method uses some reasonably slow methods of obtaining the 
      data. This should be optimised if it to be called frequently.
-------------------------------------------------------------------*/
void ModelObject::FindLimits(){
unsigned int chunkIndex;
unsigned int vertexIndex;
TriangleMeshChunk *chunk;
PackedVertex *vertexArray;

//A indicator for the first time around the loop
bool firstTime=true; 
  
  //Only proceed if we have a mesh
  if(!mesh){
     return;
  }
  
  //Loop for all chunks in the mesh
  for (chunkIndex=0;chunkIndex<mesh->GetNumChunks();chunkIndex++){

      //Get this chunk and the associated vertex array
      chunk = mesh->GetChunkArray()[chunkIndex];
      vertexArray = chunk->GetVertexArray();

      for (vertexIndex=0;vertexIndex<chunk->GetNumVertices();vertexIndex++){

          //If this is the first time just load in the values (slow code)
          if(firstTime){
             xLimits.x = vertexArray[vertexIndex].x;
             xLimits.y = vertexArray[vertexIndex].x;

             yLimits.x = vertexArray[vertexIndex].y;
             yLimits.y = vertexArray[vertexIndex].y;

             zLimits.x = vertexArray[vertexIndex].z;
             zLimits.y = vertexArray[vertexIndex].z;
             
             firstTime=false;
          }else{

             //Check "x" component
             if(vertexArray[vertexIndex].x<xLimits.x){
                xLimits.x = vertexArray[vertexIndex].x;
             }

             if(vertexArray[vertexIndex].x>xLimits.y){
                xLimits.y = vertexArray[vertexIndex].x;
             }

             //Check "y" component
             if(vertexArray[vertexIndex].y<yLimits.x){
                yLimits.x = vertexArray[vertexIndex].y;
             }

             if(vertexArray[vertexIndex].y>yLimits.y){
                yLimits.y = vertexArray[vertexIndex].y;
             }

             //Check "z" component
             if(vertexArray[vertexIndex].z<zLimits.x){
                zLimits.x = vertexArray[vertexIndex].z;
             }

             if(vertexArray[vertexIndex].z>zLimits.y){
                zLimits.y = vertexArray[vertexIndex].z;
             }
          }
      }
  }

 return;
}


/*-------------------------------------------------------------------
PROCEDURE: CentreModel
PURPOSE  : To centre the model around the orgion.
PRE : TRUE
POST: If a model is loaded, the model has been centred.

NOTE: The centring is done on the contained particle structure. 
      The outer mesh is also reset to (0,0,0)
-------------------------------------------------------------------*/
void ModelObject::CentreModel(){
float xDiff,yDiff,zDiff;
    

  //Centre "x" axis
  xDiff = (xLimits.y + xLimits.x)/2;

  //Centre "y" axis
  yDiff = (yLimits.y + yLimits.x)/2;

  //Centre "z" axis
  zDiff = (zLimits.y + zLimits.x)/2;

  this->SetPosition(Vector3(0.0f,0.0f,0.0f));

  //Set the position
  mesh->SetPosition(Vector3(-xDiff,-yDiff,-zDiff));

 return;
}
/*-------------------------------------------------------------------
PROCEDURE: IsDetailScaleable
PURPOSE  : To check to see if this model can have it's detail scaled.
PRE : TRUE
POST: If this model can be scaled, returns true else returns false.
-------------------------------------------------------------------*/
bool ModelObject::IsDetailScaleable(){

  if(object){
    return true;
  }else{
    return false;
  }

}

/*-------------------------------------------------------------------
PROCEDURE: SetAnimationSpeed
PURPOSE  : To set the speed of the current animation.
PRE : TRUE
POST: If an animation exists, the speed of it is set to "speed"
-------------------------------------------------------------------*/
void ModelObject::SetAnimationSpeed(float speed){
  if(animator){
  
    //Get the primary animation. This should always return OK  
    AnimationModifier *anim=NULL;
    if(animator->GetPrimaryAnimation(&anim)==OK){
        anim->SetSpeed(speed);
    }

  }
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetAnimationBackwords
PURPOSE  : To set the animation to play backwards.
PRE : TRUE
POST: If an animation exists, it will play backwards if "bck" = true
      else the animation will play forwards.
-------------------------------------------------------------------*/
void ModelObject::SetAnimationBackwords(bool bck){

  if(animator){
  
    //Get the primary animation. This should always return OK  
    AnimationModifier *anim=NULL;
    if(animator->GetPrimaryAnimation(&anim)==OK){

      //Set the flag that indicates backwards
      anim->SetFlag(AnimationModifier::BACKWARD,bck);
    }

  }
 
 return;
}
