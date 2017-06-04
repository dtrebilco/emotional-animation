/*--------------------------------------------------------------------
  EModifier_Rand.h

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on making random movements to skeleton bones.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#include "stdafx.h"
#include "EModifier_Rand.h"

using namespace Jet;

LOGCLIENT(EModifier_Rand);


/*-------------------------------------------------------------------
PROCEDURE: EModifier_Rand
PURPOSE  : To construct the object using default parameters.
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
EModifier_Rand::EModifier_Rand():
percentRandom(0.0f),
variance(0.0f),
minDuration(0.0f),
maxDuration(0.0f),
numBones(0),
influenceBlend(1.0f) 
{

}

/*-------------------------------------------------------------------
PROCEDURE: ~EModifier_Rand
PURPOSE  : To destruct the object. 
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
EModifier_Rand::~EModifier_Rand()
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
Modifier::Result EModifier_Rand::Update(float dt){
ParticlePtrArray::Iterator pbi;
FloatArray::Iterator bdi,bti;
Vec3Array::Iterator bai; 
Quaternion slerpOrien,origOrien,newOrien;
Vector3 origPos,tmpVec;
float rads;
float currInf;

  //Calculate the current overall influence
  currInf=influence*influenceBlend;

  //Init. iterators
  bdi=boneDuration.First(); 
  bti=boneTime.First(); 
  bai=boneAdd.First(); 

  //For each bone, alter by "influence" amount
  for (pbi=pBones->First();pbi!=pBones->End();pbi++){

      
     if(*bdi>0.0f){

         //Save Bone position and orientationm
         origOrien=pbi->GetOrientation();
         origPos  =pbi->GetPosition();

         //Increment the current orientation
         tmpVec=origOrien.GetEulerAngles();
         newOrien=Quaternion(tmpVec.x+(*bai).x,
                             tmpVec.y+(*bai).y,
                             tmpVec.z+(*bai).z);


         //Increment the time for this bone
         *bti+=dt;

         //Calculate the radians that this bone is up to (from 1.5 pi to 3.5pi)
         //rads=(*bdi*(*bti)+1.5f)*3.1415926535f;
         rads=((*bti/(*bdi))*2+1.5f)*(float)PI;

         //Create a slerp between the two orientations
         slerpOrien.Slerp(origOrien,newOrien,(sinf(rads)+1.0f)/2.0f*currInf); 
         pbi->SetOrientation(slerpOrien); 

         //LOGWARN(("Sine=: %f",(float)sin((double)rads)));

         //If the time for this bone has expired
         if(rads >= 3.142f*3.5f){
            *bdi=0.0f;
            numMoving--;
            
               //rads=(float)sin((double)rads);
         }
     }
     AssignRandomBones();

     //Increment iterators
     bdi++;
     bti++; 
     bai++;
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
bool EModifier_Rand::LoadData(EmotionModifier * em,float emotionInfluence, PString realPath,PString objectLoad){
ParticlePtrArray::Iterator pbi;

  //Load in the random data
  //percentRandom=0.3f;
  //variance=0.3f;//0.09f;
  //minDuration=1.7f;//0.01f*2.0f;
  //maxDuration=3.9f;//0.09f*2.0f;

  //percentRandom=0.3f;
  //variance=0.4f;//0.09f;
  //minDuration=2.0f;//0.01f*2.0f;
  //maxDuration=4.0f;//0.09f*2.0f;
  
  //Set the influence
  influence=emotionInfluence;

  //Save the bone structure
  pBones=em->GetBoneStruct(); 

  //Get the numebr of bones
  numBones=pBones->Count();

  //Init. the random generator
  rand.Randomise();

  //Calculate the maximum number of bones that can be moving
  maxMoving = (int)(((float)numBones) * percentRandom);
  
  //Go to each bone and add a bone variance/duration for each bone 
  for (pbi=pBones->First();pbi!=pBones->End();pbi++){
     boneDuration.Add(0.0f);
     boneTime.Add(0.0f);
     boneAdd.Add(Vector3());
  }

  //Randomly assign the number of bones that are moving
  numMoving=0;
  AssignRandomBones();
/*
  bdi=boneDuration.First();
  for (bai=boneAdd.First();bai!=boneAdd.End();bai++){
    if(numMoving >= maxMoving){
       break;
    }

    //Randomly select a bone
    if(rand.NextFloat() < percentRandom){
      (*bai).x = rand.NextFloat(-variance,variance);
      (*bai).y = rand.NextFloat(-variance,variance);
      (*bai).z = rand.NextFloat(-variance,variance);
      *bdi   = rand.NextFloat(minDuration,maxDuration); 
      numMoving++;
    }


    bdi++;
  }  
   
*/



 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: AssignRandomBones
PURPOSE  : To randomly assign the moving bones.
PRE : LoadData has been called successfully.
POST: Bones may have been assigned.
-------------------------------------------------------------------*/
void EModifier_Rand::AssignRandomBones(void){
Vector3 tmpVec;
int i,index;

  //Loop for the number of bones that are yet to be assigned
  for (i=maxMoving-numMoving;i>0;i--){

    //Randomly select a bone (skip firect 2 root bones and the hip bone)
    index=rand.NextInt(2,numBones);
    if(boneDuration[index]==0.0f&&
      (*pBones)[index]->GetName() != PString("b.hip")){
      
      //Assign bone duration, and adding vector 
      tmpVec.x = rand.NextFloat(-variance,variance);
      tmpVec.y = rand.NextFloat(-variance,variance);
      tmpVec.z = rand.NextFloat(-variance,variance);
      boneDuration[index] = rand.NextFloat(minDuration,maxDuration); 
      boneAdd[index] = tmpVec;
      boneTime[index]= 0.0f;
      numMoving++;
    }

  }  

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetPercentRandom
PURPOSE  : To set the number indicating how many bones are random.
PRE : TRUE
POST: The percent of bones moving has been set.
-------------------------------------------------------------------*/
void EModifier_Rand::SetPercentRandom(float r){
  
  //Assign the percentage
  percentRandom=r;

  //Calculate the maximum number of bones that can be moving
  maxMoving = (int)(((float)numBones) * percentRandom);

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetVariance
PURPOSE  : To set variance of the randomness.
PRE : TRUE
POST: The variance has been set.
-------------------------------------------------------------------*/
void EModifier_Rand::SetVariance(float v){
  variance=v;

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetDuration
PURPOSE  : To set how long a bone moves for.
PRE : TRUE
POST: The duration has been set.
-------------------------------------------------------------------*/
void EModifier_Rand::SetDuration(float min,float max){

  minDuration=min;
  maxDuration=max;
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetInfluenceBlend
PURPOSE  : To set the blend value for when the influence is applied.
PRE : TRUE
POST: The blend value has been set.
-------------------------------------------------------------------*/
void EModifier_Rand::SetInfluenceBlend(float ib){
  influenceBlend=ib;
 return;
}
