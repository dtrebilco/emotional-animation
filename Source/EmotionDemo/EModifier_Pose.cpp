/*--------------------------------------------------------------------
  EModifier_Pose.cpp

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on pre-defined emotion positions.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#include "stdafx.h"
#include "EModifier_Pose.h"

using namespace Jet;

LOGCLIENT(EModifier_Pose);


/*-------------------------------------------------------------------
PROCEDURE: EModifier_Pose
PURPOSE  : To construct the object using default parameters.
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
EModifier_Pose::EModifier_Pose():
influenceBlend(1.0f)
{

}

/*-------------------------------------------------------------------
PROCEDURE: ~EModifier_Pose
PURPOSE  : To destruct the object. 
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
EModifier_Pose::~EModifier_Pose()
{

}

/*-------------------------------------------------------------------
PROCEDURE: Update
PURPOSE  : Perform any required changes to the object this frame.
PRE : TRUE
POST: Returns OK if no problems were encountered.
      Modifier::RES_REMOVE  if the host should remove this modifier.
      Modifier::RES_REMOVE_DESTROY if host should remove and destroy
      this modifier.
-------------------------------------------------------------------*/
Modifier::Result EModifier_Pose::Update(float dt){
  
ParticlePtrArray::Iterator pbi;
QuatArray::Iterator noi;
Vec3Array::Iterator npi;
FloatArray::Iterator orienIi,posIi;
Quaternion slerpOrien,origOrien;
Vector3 origPos;
float currInf;

  //Calculate the current overall influence
  currInf=influence*influenceBlend;

  //If we have not init yet
  if(!init){
      return OK;
  }

  //Init. iterators
  noi=poseOrients.First();
  npi=posePositions.First(); 
  orienIi=orientationInf.First(); 
  posIi  =positionInf.First(); 

  //For each bone, alter by "influence" amount
  for (pbi=pBones->First();pbi!=pBones->End();pbi++){
     
     //Save Bone position and orientationm
     origOrien=pbi->GetOrientation();
     origPos  =pbi->GetPosition();

     //Create a slerp between the two orientations
     if(*orienIi > 0.0f){
       slerpOrien.Slerp(origOrien,*noi,currInf*(*orienIi)); 
       pbi->SetOrientation(slerpOrien); 
       //pbi->SetOrientation(((pbi->GetOrientation()-*noi)*(1-currInf)) + *noi);
     }
     
     if(*posIi > 0.0f){
       pbi->SetPosition(((origPos-*npi)*(1-(currInf*(*posIi)))) + *npi); 
     }

     //Increment iterators
     noi++;
     npi++;
     posIi++;
     orienIi++;
  }

 return OK;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadData
PURPOSE  : To load the data for this modifier.
PRE : em is valid. (ie not NULL)
POST: Returns true if the modifier is loaded successfully, 
      false if not.
-------------------------------------------------------------------*/
bool EModifier_Pose::LoadData(EmotionModifier * em,float emotionInfluence, PString realPath,PString objectLoad){
ParticlePtrArray::Iterator pbi;

  //Set the influence
  influence=emotionInfluence;
  
  //Sets how to decide what is "significant" when comparing the pose against the neural stance
  orientationInfLimit = 0.01f;
  positionInfLimit    = 0.0001f;

  //Save the bone structure
  pBones=em->GetBoneStruct(); 

  //Go to each bone and assume the current position is neutral and orientation is 0,0,0
  for (pbi=pBones->First();pbi!=pBones->End();pbi++){
     
     //Save the orientation and position
     poseOrients.Add  (Quaternion(0.0f,0.0f,0.0f));
     posePositions.Add(Vector3(pbi->GetPosition()));
     
     //Setup the influence for position and orientation
     positionInf.Add(0.0f); 
     orientationInf.Add(0.0f); 
  }

  
  //Load in the (optional) neutral stance
  if(objectLoad!=PString("")){
    LoadNeutralSkeleton(realPath+"/"+objectLoad+"/neutral.ske");

    //Try to load the pose data
    if(LoadPoseSkeleton(realPath+"/"+objectLoad+"/pose.ske")==0){
       return false;
    }
  
  }else{
     //We can only init with specific data
     return false;
  }

  //Set the init flag
  init=true;

 return true;
}


/*-------------------------------------------------------------------
PROCEDURE: LoadNeutralSkeleton
PURPOSE  : To load the netural skeleton
PRE : TRUE
POST: Returns the number of bones that were loaded from the file.

NOTE: Can be optimised by better use of arrays, and checking is not 
      done on every read. 
-------------------------------------------------------------------*/
unsigned int EModifier_Pose::LoadNeutralSkeleton(PString fileName){
uword i,s,numVertices=0,loadedVertices;
Vector3    tmpPos;
Quaternion tmpOrien;

ParticlePtrArray::Iterator pbi;
PString   boneName;
int countPos;
File inFile;


  //Try to open the input file 
  if (!inFile.Open(fileName, READ_RAW | OTYPE_BINARY)){
    LOGERR(("EModifier_Pose::LoadNeutralSkeleton() Unable to open load file"));
    return 0;
  }

  //Read in data mode
  if (!inFile.Read(s)){
    LOGERR(("EModifier_Pose::LoadNeutralSkeleton() Unable to read file type"));
    return 0;
  }

  //Read in the number of vertices
  if (!inFile.Read(numVertices)){
    LOGERR(("EModifier_Pose::LoadNeutralSkeleton() Unable to read the number of vertices"));
    return 0;
  }
  loadedVertices=0;

  //Read in each bones's data
  for(i=0;i<numVertices;i++){
  
     //Read in the bone name 
     inFile.Read(boneName); 

     //Find the bone
     countPos=0;
     for (pbi=pBones->First();pbi!=pBones->End();pbi++){
       if(pbi->GetName()==boneName){
         break;
       }
       countPos++;
     }

     //Read in all the bone data ModifierList BoneBlender
     if(pbi!=pBones->End()){
       loadedVertices++;
       
       //Read in the bone position
       if(s==SKELE_POSITION || s==SKELE_BOTH){    

         //Read in out the data
         inFile.Read(tmpPos.x);
         inFile.Read(tmpPos.y);
         inFile.Read(tmpPos.z);

         posePositions[countPos]=tmpPos; 
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
         //slerpOrien.Slerp(currBone->GetOrientation(),tmpOrien,0.5f);
         //pbi->SetOrientation(tmpOrien); 
         poseOrients[countPos]=tmpOrien;
       }

     }
  }

  //Close the file
  inFile.Close(); 

 return loadedVertices;
}


/*-------------------------------------------------------------------
PROCEDURE: LoadPoseSkeleton
PURPOSE  : To load the pose skeleton
PRE : TRUE
POST: Returns the number of bones that were loaded from the file.

NOTE: Can be optimised by better use of arrays, and checking is not 
      done on every read. 
-------------------------------------------------------------------*/
unsigned int EModifier_Pose::LoadPoseSkeleton(PString fileName){
uword i,s,numVertices=0,loadedVertices;
Vector3    tmpPos;
Quaternion tmpOrien;

ParticlePtrArray::Iterator pbi;
PString   boneName;
int countPos;
float tmpCmp;
File inFile;


  //Try to open the input file 
  if (!inFile.Open(fileName, READ_RAW | OTYPE_BINARY)){
    LOGERR((" EModifier_Pose::LoadPoseSkeleton() Unable to open load file"));
    return 0;
  }

  //Read in data mode
  if (!inFile.Read(s)){
    LOGERR((" EModifier_Pose::LoadPoseSkeleton() Unable to read file type"));
    return 0;
  }

  //Read in the number of vertices
  if (!inFile.Read(numVertices)){
    LOGERR((" EModifier_Pose::LoadPoseSkeleton() Unable to read the number of vertices"));
    return 0;
  }
  loadedVertices=0;

  //Read in each bones's data
  for(i=0;i<numVertices;i++){
  
     //Read in the bone name 
     inFile.Read(boneName); 

     //Find the bone
     countPos=0;
     for (pbi=pBones->First();pbi!=pBones->End();pbi++){
       if(pbi->GetName()==boneName){
         break;
       }
       countPos++;
     }

     //Read in all the bone data ModifierList BoneBlender
     if(pbi!=pBones->End()){
       loadedVertices++;
       
       //Read in the bone position
       if(s==SKELE_POSITION || s==SKELE_BOTH){    

         //Read in out the data
         inFile.Read(tmpPos.x);
         inFile.Read(tmpPos.y);
         inFile.Read(tmpPos.z);
         
         //Test if the value is greater then the limit
         tmpCmp=(tmpPos-posePositions[countPos]).MagnitudeSquared();
         if(tmpCmp > 0.001){
            posePositions[countPos]=tmpPos; 
            positionInf[countPos]  = tmpCmp * 2.0f;
            if(positionInf[countPos] >1.0f){
               positionInf[countPos]=1.0f;
            }
         }
       }

       //Read in the bone orientation
       if(s==SKELE_ORIENTATION || s==SKELE_BOTH){
                    
         //Read in the data
         inFile.Read(tmpOrien.v.x);
         inFile.Read(tmpOrien.v.y);
         inFile.Read(tmpOrien.v.z);
         inFile.Read(tmpOrien.w);

         //Test if this value is greater than the limit
         tmpCmp=(tmpOrien.GetEulerAngles() - poseOrients[countPos].GetEulerAngles()).MagnitudeSquared();
         if(tmpCmp > 0.001f){
            poseOrients[countPos]=tmpOrien;
            orientationInf[countPos]=tmpCmp *2.0f;
            if(orientationInf[countPos] > 1.0f){
               orientationInf[countPos] =1.0f;
            }
         }
       }

     }
  }

  //Close the file
  inFile.Close(); 

 return loadedVertices;
}


/*-------------------------------------------------------------------
PROCEDURE: SetInfluenceBlend
PURPOSE  : To set the blend value for when the influence is applied.
PRE : TRUE
POST: The blend value has been set.
-------------------------------------------------------------------*/
void EModifier_Pose::SetInfluenceBlend(float ib){
  influenceBlend=ib;
 return;
}
