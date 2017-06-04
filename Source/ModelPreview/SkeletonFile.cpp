/*--------------------------------------------------------------------
  SkeletonFile.h

  Contains the implementation of the class that allows the 
  saving and loading of the skeletal data in a particle structure.

  31/03/2001 -- Created the file

  NOTE: Some methods don't perform full error checking.

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "StdAfx.h"
#include "SkeletonFile.h"

using namespace Jet;

LOGCLIENT(SkeletonFile);

/*-------------------------------------------------------------------
PROCEDURE: SkeletonFile
PURPOSE  : To construct the object
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
SkeletonFile::SkeletonFile(){

}

/*-------------------------------------------------------------------
PROCEDURE: ~SkeletonFile
PURPOSE  : To destruct the object
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
SkeletonFile::~SkeletonFile(){

}

/*-------------------------------------------------------------------
PROCEDURE: LoadSkeleton
PURPOSE  : To load a previously saved particle structure bone file
PRE : "ps" points to a valid particle structure
POST: Returns the number of bones that were loaded from the file.

NOTE: Can be optimised by better use of arrays, and checking is not 
      done on every read.
-------------------------------------------------------------------*/
unsigned int SkeletonFile::LoadSkeleton(ParticleStructure *ps,PString fileName){
uword i,s,numVertices=0,loadedVertices;
Vector3    tmpPos;
Quaternion tmpOrien,slerpOrien;

PString   boneName;
Particle *currBone;
File inFile;

  //Try to open the input file 
  if (!inFile.Open(fileName, READ_RAW | OTYPE_BINARY)){
    LOGERR(("SkeletonFile::LoadSkeleton() Unable to open load file"));
    return 0;
  }

  //Read in data mode
  if (!inFile.Read(s)){
    LOGERR(("SkeletonFile::LoadSkeleton() Unable to read file type"));
    return 0;
  }

  //Read in the number of vertices
  if (!inFile.Read(numVertices)){
    LOGERR(("SkeletonFile::LoadSkeleton() Unable to read the number of vertices"));
    return 0;
  }
  loadedVertices=0;

  //Read in each bones's data
  for(i=0;i<numVertices;i++){
  
     //Read in the bone name 
     inFile.Read(boneName); 

     //Find the bone
     currBone = ps->FindByName(boneName);
        
     //Read in all the bone data ModifierList BoneBlender
     if(currBone!=NULL){
       loadedVertices++;
       
       //Read in the bone position
       if(s==SKELE_POSITION || s==SKELE_BOTH){    

         //Read in out the data
         inFile.Read(tmpPos.x);
         inFile.Read(tmpPos.y);
         inFile.Read(tmpPos.z);

         currBone->SetPosition(tmpPos); 
       }

       //Read in the bone orientation
       if(s==SKELE_ORIENTATION || s==SKELE_BOTH){
                    
         //Read in the data
         inFile.Read(tmpOrien.v.x);
         inFile.Read(tmpOrien.v.y);
         inFile.Read(tmpOrien.v.z);
         inFile.Read(tmpOrien.w);

         //currBone->SetOrientation(tmpOrien); 
         //tmpOrien=currBone->GetOrientation(); 
         slerpOrien.Slerp(currBone->GetOrientation(),tmpOrien,0.5f);
         currBone->SetOrientation(slerpOrien); 
       }

     }
  }

  //Close the file
  inFile.Close(); 

 return loadedVertices;
}

/*-------------------------------------------------------------------
PROCEDURE: SaveSkeleton
PURPOSE  : To save the position/orientation of the bone structure of 
           the provided particle structure.
PRE : "ps" points to a valid particle structure
POST: Depending on the value of "s" the position and/or orientation 
      is saved to file "filename". Returns the number of bones saved.

NOTE: Can be optimised by better use of arrays, and checking is not 
      done on every write.
-------------------------------------------------------------------*/
unsigned int SkeletonFile::SaveSkeleton(ParticleStructure *ps,PString fileName,SKELE_DATA_SAVED s){
unsigned int i,numVertices;
ParticlePtrArray pArray;
Vector3    tmpPos;
Quaternion tmpOrien;
File outFile;

  //Check the save format
  if(s<SKELE_ORIENTATION || s>SKELE_BOTH){
    LOGERR(("SkeletonFile::SaveSkeleton() Invalid data format"));
    return 0;
  }

  //Try to open the output file
  if (!outFile.Open(fileName, OPEN_CREATE | WRITE_RAW | OTYPE_BINARY)){
    LOGERR(("SkeletonFile::SaveSkeleton() Unable to open save file"));
    return 0;
  }

  //Get the bone particles
  GetBones(ps,pArray);   

  //Set the number of bones
  numVertices= pArray.Count();

  //Write out the data mode
  if (!outFile.Write((uword)s)){
    LOGERR(("SkeletonFile::SaveSkeleton() Unable to write file type"));
    return 0;
  }
  
  //Write out the number of vertices
  if(!outFile.Write((uword)numVertices)){ 
    LOGERR(("SkeletonFile::SaveSkeleton() Unable to write number of vertices"));
    return 0;
  }

  //Loop and write out each bone's data (assume data is written OK)
  for(i=0;i<numVertices;i++){

    //Write out the bone name
    outFile.Write(pArray[i]->GetName());

    //Write out bone position
    if(s==SKELE_POSITION || s==SKELE_BOTH){    

      //Get the position 
      tmpPos=pArray[i]->GetPosition();

      //Write out the data
      outFile.Write(tmpPos.x);
      outFile.Write(tmpPos.y);
      outFile.Write(tmpPos.z);
    }

    //Write out bone orientation
    if(s==SKELE_ORIENTATION || s==SKELE_BOTH){
      
      //Get the orientation
      tmpOrien=pArray[i]->GetOrientation(); 

      //Write out the data
      outFile.Write(tmpOrien.v.x);
      outFile.Write(tmpOrien.v.y);
      outFile.Write(tmpOrien.v.z);
      outFile.Write(tmpOrien.w);
    }
  }

  //Close the output file
 outFile.Close();

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
void SkeletonFile::GetBones(Particle *p,ParticlePtrArray &pArray) {
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
       GetBones(p->GetChild(i),pArray);
   }

 return;
}


