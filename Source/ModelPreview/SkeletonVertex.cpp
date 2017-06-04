/*--------------------------------------------------------------------
  SkeletonVertex.cpp

  Contains the implementation of the class that allows the 
  displaying of a ParticleStructure Skeletion

  09/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "StdAfx.h"
#include "SkeletonVertex.h"

using namespace Jet;

LOGCLIENT(SkeletonVertex);

/*-------------------------------------------------------------------
PROCEDURE: SkeletonVertex
PURPOSE  : To construct the object
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
SkeletonVertex::SkeletonVertex():
numVertices(0),
pStruct(NULL),
showGeometry(NULL),
scale(1.0f),
selectedIndex(-1)
{

}

/*-------------------------------------------------------------------
PROCEDURE: ~SkeletonVertex
PURPOSE  : To destruct the object
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
SkeletonVertex::~SkeletonVertex(){

  //Just unload particle structure data
  UnLoadParticleStructure();
}


/*-------------------------------------------------------------------
PROCEDURE: LoadParticleStructure
PURPOSE  : To Load a particle structure's bone data. 
PRE : ps points to a valid ParticleStructure. (see below)
POST: The bone data in the particle structure has been loaded and is 
      ready for rendering. If a previous particle structure was 
      loaded, it is unloaded. "show" is the name of
      mesh resources in "db" that will be loaded for each vertex in 
      the bone structure. 

NOTE: It is important to note that the particle structure MUST 
      have a finite beginning and end or this function may consume all 
      available memory then crash.
-------------------------------------------------------------------*/
unsigned int SkeletonVertex::LoadParticleStructure(RenderKernel *rk,ParticleStructure *ps,ResourceDatabase *db,PString show){

  //Free pre-existing data
  UnLoadParticleStructure();

  //Set the render kernel
  renderKernel=rk;

  //Set the particle structure
  pStruct=ps;

  //Get the particles that are bones
  GetBones(ps);

  //Set the number of bones
  numVertices= pArray.Count();

  //Load the normal "show" mesh
  showGeometry=NewMem(GeometryBaseInstance());
  showGeometry->Init(db, show);

  //If we cannot load the vertex meshes
  if(!LoadVertexMeshes()){
     LOGERR(("SkeletonVertex::LoadParticleStructure() Unable to load show mesh"));
     RemoveVertexMeshes();
     return 0;
  }

 return numVertices;
}

/*-------------------------------------------------------------------
PROCEDURE: UnLoadParticleStructure
PURPOSE  : To free all memory associated with a previous 
           ParticleStructue loading. However this can be called at 
           any time.
PRE : TRUE
POST: All data previously associated with a ParticleStructure has 
      been removed.

NOTE: This does not remove the meshes loaded on each vertex bone of 
      a particle structure, it only removes the references to a 
      previous particle structure. Call RemoveVertexMeshes to 
      remove the vertex meshes. 
-------------------------------------------------------------------*/
void SkeletonVertex::UnLoadParticleStructure(){

  //Free Pointers to Particles
  pArray.RemoveAll();
  
  //Remove all the old references to the old mesh
  pStrArray.RemoveAll();  

  //Set number of vertices to zero
  numVertices=0;

  //Re-set particle structure
  pStruct=NULL;

  if(showGeometry){
      DeleteMemZ(showGeometry);
  }

  scale=1.0f;
  selectedIndex=-1;

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: GetNumVertices
PURPOSE  : To get the number of vertices that have been loaded in from 
           the current ParticleStructure.
PRE : TRUE
POST: The number of vertices (if any) are returned
-------------------------------------------------------------------*/
unsigned int SkeletonVertex::GetNumVertices()const{
 
 return numVertices;
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
void SkeletonVertex::GetBones(Particle *p) {
unsigned int i; 
unsigned int numChild;
    
   //Get the first 2 characters of this bone's name
   PString pname=p->GetName();
   pname.Left(2);
     
   //If this particle is a bone,  
   if(pname.Compare("b.")==0){

     //Add the bone to the array
     pArray.Add(p);
   }
   
    
   //Call on this particles children 
   numChild=p->GetNumChildren();
   for(i=0;i<numChild;i++){
       //Recursive call
       GetBones(p->GetChild(i));
   }

 return;
}


/*-------------------------------------------------------------------
PROCEDURE: RemoveVertexMeshes
PURPOSE  : To remove the loaded vertex meshes and unload the particle 
           structure. 
PRE : TRUE
POST: If the current particle structure has any meshes loaded on it's 
      vertices, they are removed. In addition, the current particle 
      structure is unloaded.

NOTE: Do NOT call this method if you have already deleted the loaded 
      particle structure. Doing that automatically deletes the vertex 
      meshes.
-------------------------------------------------------------------*/
void SkeletonVertex::RemoveVertexMeshes(){
unsigned int i;

 //Loop and delete all allocated memory
 for(i=0;i<numVertices;i++){
    DeleteMem(pStrArray[i]);
 }

 //Remove all the deleted elements
 pStrArray.RemoveAll();  

 //Unload the rest of the data
 UnLoadParticleStructure();

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadVertexMeshes
PURPOSE  : To load vertex meshes for the current particle structure.
PRE : TRUE
POST: The particle structure has meshes from "showGeometry" loaded 
      in each vertex.
-------------------------------------------------------------------*/
bool SkeletonVertex::LoadVertexMeshes(){
unsigned int i;
ParticleStructure *ps;

  //Do not proceed if we do not have any meshes to load
  if(!showGeometry){
     return false;
  }

  //Loop for all bone vertices
  for(i=0;i<numVertices;i++){
       
    //Create a loading structure for the mesh
    ps = NewMem(ParticleStructure());
    if (!showGeometry->CreateInstance(renderKernel, ps)){
       return false;
    }

    //Scale the loaded mesh
    ps->SetScale(Vector3(scale,scale,scale)); 

    //Add it to our particle structure reference array
    pStrArray.Add(ps);

    //Add it to the bone
    pArray[i]->Add(ps);
  }
   

 return true;
}


/*-------------------------------------------------------------------
PROCEDURE: SetScale
PURPOSE  : To set the scaling factor for the vertex meshes.
PRE : TRUE
POST: The scaling factor is set and if there is a currently loaded 
      particle structure, the scaling is applied to it's vertex 
      meshes.
-------------------------------------------------------------------*/
void SkeletonVertex::SetScale(float s){
unsigned int i;

  //Assign the scaling factor
  scale=s;

  //Apply it to the existing particle structure
  for(i=0;i<numVertices;i++){
     pStrArray[i]->SetScale(Vector3(scale,scale,scale));
  }

 return; 
}

/*-------------------------------------------------------------------
PROCEDURE: TestCollision
PURPOSE  : To test if a line collides with any of the vertex meshes 
           and if so, selects the mesh using the previously loaded 
           selection mesh. If a previous mesh was selected it is 
           unselected. The optional "refPos" is used to decide 
           if two or meshs are found. Then the one found closest to 
           "refPos" is taken
PRE : TRUE
POST: If a mesh intersects "testline", it is selected and the 
      previous mesh is unselected. If no mesh was selected, the 
      previous selected mesh is still unselected. Returns true on 
      a successful selection.

TODO: The loop uses indexes which is slow in a FastDynamicArray.
-------------------------------------------------------------------*/
bool SkeletonVertex::TestCollision(Line testLine,Vector3 refPos){
bool collision = false;
//CollisionResult result;
Vector3 posVector;

//The compare distances
float prevDist= 1.0e5;
float newDist;

//The index of the collision 
int collIndex=-1;
unsigned int i;

  //Init. the result
  //result.Initialise();

  //Loop for all meshes
  for (i=0;i<numVertices;i++){

    //This hack exists until Jet fixes bounding volume resizing problems
    posVector.Zero();
    pStrArray[i]->LocalToGlobal(posVector);            

    //Perform the test against the line
    if(testLine.DistanceSquared(posVector) < 0.06f*0.06f){
      collision = true;

      //Take the nearest one (approx method)
      newDist=(refPos-posVector).Magnitude();
      if(newDist<prevDist){
        prevDist=newDist;
        collIndex=i;
      }

    }

    /*
    //If a collision occurs
    if (pStrArray[i]->TestCollision(testLine, result)){
      collision = true;
      
      //Calculate the distance to the reference point and compare
      //  it to the previous distance
      //newDist=(refPos-result.point).Magnitude();
      //if(newDist<prevDist){
         prevDist=newDist;
         collIndex=i;
      //}
      break;
    }
    */
  }

  //If we are selecting the same object
  if(collision && collIndex==selectedIndex){
      return true;
  }

  //Remove the previous selected object
  if(selectedIndex>=0){
    pStrArray[selectedIndex]->SetOpacity(1.0f);

    //Change the colour of the mesh back
    IndexedMeshInstance *mesh =safe_cast<IndexedMeshInstance *>(pStrArray[selectedIndex]->GetChild(0));
    unsigned int numChunks =mesh->GetNumChunks();
    TriangleMeshChunk **chunkArray = mesh->GetChunkArray();

    // Work through the chunks and set the ambient color
    for (uint index = 0; index < numChunks; index++){
      chunkArray[index]->GetMaterial()->ambientCol = Color(1.0f,1.0f,1.0f);

      // Recalculate ambient color of the chunks
      // This will be automatically performed by the LightTarget class demonstrated
      // in the lighting examples
      chunkArray[index]->ResetToAmbient(renderKernel);
    }
   

    selectedIndex=-1;
  }


  //If we had a collision create the selected mesh
  if(collision){

    //pStrArray[collIndex]->SetOpacity(0.5f);
    selectedIndex=collIndex;

    //Change colour of the mesh
    IndexedMeshInstance *mesh =safe_cast<IndexedMeshInstance *>(pStrArray[collIndex]->GetChild(0));
    unsigned int numChunks =mesh->GetNumChunks();
    TriangleMeshChunk **chunkArray = mesh->GetChunkArray();

    // Work through the chunks and set the tint (This is just estimation)
    for (uint index = 0; index < numChunks; index++){
      chunkArray[index]->Tint(renderKernel,Color(0.5f,0.8f,0.0f));
    }

    return true;
  }


 return false;
}


/*-------------------------------------------------------------------
PROCEDURE: IsVertexSelected
PURPOSE  : To test if there is a vertex currently selected
PRE : TRUE
POST: Returns true if there is, false if not
-------------------------------------------------------------------*/
bool SkeletonVertex::IsVertexSelected(){

  //Test for a valid index
  if(selectedIndex>-1){
    return true;
  }else{
    return false;
  }
}


/*-------------------------------------------------------------------
PROCEDURE: GetVertexName
PURPOSE  : To get the name of the selected vertex. 
PRE : TRUE
POST: If no vertex is currently selected, returns "None"
-------------------------------------------------------------------*/
PString SkeletonVertex::GetVertexName(){

  //Test if we have a selected vertex
  if(IsVertexSelected()){
     return pArray[selectedIndex]->GetName(); 
  }else{
     return PString("None");
  }


}

/*-------------------------------------------------------------------
PROCEDURE: GetVertexOrientation
PURPOSE  : To get the orientation of the currenly selected vertex 
           (Euler Angles)
PRE : TRUE
POST: If no vertex is currently selected, returns 0,0,0
-------------------------------------------------------------------*/
Vector3 SkeletonVertex::GetVertexOrientation(){

  //Test if we have a selected vertex
  if(IsVertexSelected()){
     return pArray[selectedIndex]->GetEulerAngles(); 
  }else{
     return Vector3(0.0f,0.0f,0.0f);
  }

}

/*-------------------------------------------------------------------
PROCEDURE: SetVertexOrientation
PURPOSE  : To set the orientation of the selected vertex 
           (Euler Angles)
PRE : TRUE
POST: If the orientation was set, returns true, else returns false.
-------------------------------------------------------------------*/
bool SkeletonVertex::SetVertexOrientation(Vector3 newOrien){

  //Test if we have a selected vertex
  if(IsVertexSelected()){
    pArray[selectedIndex]->SetEulerAngles(newOrien.x,newOrien.y,newOrien.z); 
    return true;
  }else{
    return false;
  }
}
