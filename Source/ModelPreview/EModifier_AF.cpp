/*--------------------------------------------------------------------
  EModifier_AF.cpp

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on amplitude and frequency movements.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#include "stdafx.h"
#include "EModifier_AF.h"

using namespace Jet;

LOGCLIENT(EModifier_AF);


/*-------------------------------------------------------------------
PROCEDURE: EModifier_AF
PURPOSE  : To construct the object using default parameters.
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
EModifier_AF::EModifier_AF()
{

}

/*-------------------------------------------------------------------
PROCEDURE: ~EModifier_AF
PURPOSE  : To destruct the object. 
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
EModifier_AF::~EModifier_AF()
{

}


/*-------------------------------------------------------------------
PROCEDURE: SetInfluence
PURPOSE  : To set the influence of this object. 
PRE : TRUE
POST: The influence is set. (Influences that are negative and are 
      > 1.0 are acceptable but may produce strange results)
-------------------------------------------------------------------*/
void EModifier_AF::SetInfluence(float inf){

  //Call the parent
  EModifier::SetInfluence(inf); 

  //Reset the frequency
  SetFrequency(freqInfluence);

  //Reset the amplitude
  SetAmplitude(ampInfluence);

 return; 
}

/*-------------------------------------------------------------------
PROCEDURE: SetFrequency
PURPOSE  : To set the frequency (animation speed) value. 
PRE : TRUE
POST: The frequency value is set. (scaled by influence)
-------------------------------------------------------------------*/
void EModifier_AF::SetFrequency(float freq){
ModifierArray::Iterator modi;
ModifierArray * modArray;
AnimationModifier *am;

  //Assign the frequency    
  freqInfluence=freq;

  //Scale current frequency by the influence
  currFreqInf=freqInfluence+(1.0f-freqInfluence)*(1.0f-influence);
  
  //Set the frequency value by modifying the animation speeds
  modArray=emotionMod->GetHost()->GetModifiers();  
  for(modi=modArray->First();modi!=modArray->End();modi++){
      
      //Cast the animation type
      if(am=safe_cast<AnimationModifier *>(*modi)){
        if(freqMix){
          am->SetSpeed(am->GetSpeed()*currFreqInf);  
        }else{
          am->SetSpeed(currFreqInf);  
        }
      }

  }

 return; 
}

/*-------------------------------------------------------------------
PROCEDURE: SetAmplitude
PURPOSE  : To set the amplitude variance of this modifier. 
PRE : TRUE
POST: The amplitude value is set. (Scaled by the influence)
-------------------------------------------------------------------*/
void EModifier_AF::SetAmplitude(float amp ){

  //Assigne the amplitude  
  ampInfluence=amp;

  //Scale current frequency by the influence
  currAmpInf =ampInfluence*influence;

 return; 
}

/*-------------------------------------------------------------------
PROCEDURE: SetFreqMix
PURPOSE  : To set weither adjusting the frequency (or influence) 
           mixes with the current frequency.
PRE : TRUE
POST: The max value is set
-------------------------------------------------------------------*/
void EModifier_AF::SetFreqMix(bool  mix ){
  freqMix=mix;
  SetFrequency(freqInfluence);
 return; 
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
Modifier::Result EModifier_AF::Update(float dt){
ParticlePtrArray::Iterator pbi;
QuatArray::Iterator noi;
Vec3Array::Iterator npi;
FloatArray::Iterator nmi;
Quaternion slerpOrien,origOrien;
Vector3 origPos;

  //Init. iterators
  noi=neutralOrients.First();
  npi=neutralPositions.First(); 
  nmi=neutralMagnitudes.First();

  //For each bone, alter by "influence" amount
  for (pbi=pBones->First();pbi!=pBones->End();pbi++){
     
     //Save Bone position and orientationm
     origOrien=pbi->GetOrientation();
     origPos  =pbi->GetPosition();

     
     //Create a slerp between the two orientations
     slerpOrien.Slerp(origOrien,*noi,currAmpInf); 
     
     pbi->SetOrientation(slerpOrien); 
     //pbi->SetOrientation(((pbi->GetOrientation()-*noi)*(1-influence)) + *noi);

     pbi->SetPosition(((origPos-*npi)*(1-currAmpInf)) + *npi); 

     //Re-set neutral orientation/position
     //slerpOrien.Slerp(*noi,origOrien,0.001f); 
     //*noi=slerpOrien;
     //*npi=((origPos-*npi)*(0.001f)) + *npi;
 
     
     float val=(float)(fabs(slerpOrien.GetEulerAngles().x)+fabs(slerpOrien.GetEulerAngles().y)+
                       fabs(slerpOrien.GetEulerAngles().z));
     if(val < *nmi){
        *nmi=val;
        *noi=slerpOrien;
        *npi=pbi->GetPosition();
     }
     
     
     //*noi=slerpOrien;
     //*npi=pbi->GetPosition();


     //Increment iterators
     noi++;
     npi++;
     nmi++;
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
bool EModifier_AF::LoadData(EmotionModifier * em,float emotionInfluence, PString realPath,PString objectLoad){
ParticlePtrArray::Iterator pbi;

  //Save the bone structure
  pBones=em->GetBoneStruct(); 

  //Save the emotion modifier
  emotionMod = em;

  //Set the frequency and amplitude
  freqInfluence= 1.60f;
  ampInfluence = 0.6f;
  freqMix=true;

  //Set the overall influence
  SetInfluence(emotionInfluence);

  //Empty all arrays incase this is called more than once
  neutralOrients.RemoveAll();
  neutralPositions.RemoveAll();
  neutralMagnitudes.RemoveAll();

  //Go to each bone and assume the current position is neutral
  for (pbi=pBones->First();pbi!=pBones->End();pbi++){
     
     //Save the orientation and position
     neutralOrients.Add  (Quaternion(pbi->GetOrientation()));
     neutralPositions.Add(Vector3(pbi->GetPosition()));
     
     float val=(float)(fabs(pbi->GetOrientation().GetEulerAngles().x)+fabs(pbi->GetOrientation().GetEulerAngles().y)+
                       fabs(pbi->GetOrientation().GetEulerAngles().z));

     neutralMagnitudes.Add(val); 
  }

/*  
  //Load in the (optional) neutral stance
  if(objectLoad!=PString("")){
    LoadNeutralSkeleton(realPath+"/"+objectLoad+"/neutral.ske");
  }
  //*/
 
    
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
unsigned int EModifier_AF::LoadNeutralSkeleton(PString fileName){
uword i,s,numVertices=0,loadedVertices;
Vector3    tmpPos;
Quaternion tmpOrien;

ParticlePtrArray::Iterator pbi;
PString   boneName;
int countPos;
File inFile;


  //Try to open the input file 
  if (!inFile.Open(fileName, READ_RAW | OTYPE_BINARY)){
    LOGERR(("EModifier_AF::LoadNeutralSkeleton() Unable to open load file"));
    return 0;
  }

  //Read in data mode
  if (!inFile.Read(s)){
    LOGERR(("EModifier_AF::LoadNeutralSkeleton() Unable to read file type"));
    return 0;
  }

  //Read in the number of vertices
  if (!inFile.Read(numVertices)){
    LOGERR(("EModifier_AF::LoadNeutralSkeleton() Unable to read the number of vertices"));
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

         neutralPositions[countPos]=tmpPos; 
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
         neutralOrients[countPos]=tmpOrien;
       }

     }
  }

  //Close the file
  inFile.Close(); 

 return loadedVertices;
}
