/*--------------------------------------------------------------------
  VEInferenceEngine.h

  Contains the virtual environment emotion inference engine.

  08/06/2001 -- Created the file

DISCLAIMER: This inference engine is only a simple demostration
            and uses the emotion modifiers in ways not intended.
            (ie. Loading 4 separate emotions on one character and 
             altering each emotion influence)

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#include "stdafx.h"
#include "VEInferenceEngine.h"

using namespace Jet;

LOGCLIENT(VEInferenceEngine);

//Global random generateor
extern Rand randG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
VEInferenceEngine::VEInferenceEngine():
globalTarget(0.0f),
globalInfluence(0.0f)
{

}

VEInferenceEngine::~VEInferenceEngine(){
unsigned int i,i2;

  //Release emotions
  i2=E_Angry.Count();

  for(i=0;i<i2;i++){
    
    //Get the skeleton modifier
    //EmotionSkeletonModifier *esm=(*emotionC)[i]->GetEmotionSkeletonModifier();
    DeleteMem(E_Angry[i]);
    DeleteMem(E_Calm[i]);
    DeleteMem(E_Depressed[i]);
    DeleteMem(E_Happy[i]);

    //Remove all modifiers
    //esm->RemoveEmotionModifier(E_Angry[i]);   
    //esm->RemoveEmotionModifier(E_Calm[i]);   
    //esm->RemoveEmotionModifier(E_Depressed[i]);   
    //esm->RemoveEmotionModifier(E_Happy[i]);   
  }

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
Modifier::Result VEInferenceEngine::Update(float dt){
unsigned int index;

  //Randomly alter 1 character
  index=randG.NextInt(emotionTarget.Count());
  ModifyEmotionCharacter(index,
                         emotionTarget[index]   +randG.NextFloat(-25.0f,25.0f),
                         emotionInfTarget[index]+randG.NextFloat( -0.1f,0.1f ));  

 return OK;
}

/*-------------------------------------------------------------------
PROCEDURE: Init
PURPOSE  : To load all the characters in "emotionC" with emotions 
           between emotionMin and emotionMax and be assigned a randGom 
           influence.
PRE : 0<= emoMin,emomax <360 , Init has not been call previously
POST: All characters have been assigned emotions.
-------------------------------------------------------------------*/
bool VEInferenceEngine::Init(CharacterArrayPointer emotionC,float emotionMin,float emotionMax){
unsigned int i;
EModifier_AF   * modAF;
//EModifier_Grav * modGrav;
EModifier_Pose * modPose;
EModifier_Rand * modRand;

  //Assign a pointer to the emotion characters
  emoC=emotionC;

  //The the speeds for each emotion
  emotionSpeeds[0]=4.5f; //Angry
  emotionSpeeds[1]=1.5f; //Calm
  emotionSpeeds[2]=0.7f; //Depressed 
  emotionSpeeds[3]=2.6f; //Happy

  //Loop for all characters
  for(i=0;i<emotionC->Count();i++){
     
      //Create all emotions (should check for memory non-allocation)
      EmotionSkeletonModifier *esm=(*emotionC)[i]->GetEmotionSkeletonModifier();

      E_Angry.Add    (NewMem(EmotionModifier(esm)));
      E_Calm.Add     (NewMem(EmotionModifier(esm)));
      E_Depressed.Add(NewMem(EmotionModifier(esm)));
      E_Happy.Add    (NewMem(EmotionModifier(esm)));

      emotionTarget.Add(0.0f);
      emotionInfTarget.Add(1.0f);
      //Init all emotions

      //Init Angry
      E_Angry[i]->LoadModifier(MOD_POSE,1.00f,"Emotions","Angry","Human"); 
      E_Angry[i]->LoadModifier(MOD_AF  ,1.00f,"Emotions","Angry","Human"); 
      E_Angry[i]->LoadModifier(MOD_RAND,1.00f,"Emotions","Angry","Human"); 
      //E_Angry[i]->LoadModifier(MOD_GRAV  ,0.50f,"Emotions","Angry","Human"); 

          //Manually set data (hacky)
          modAF  =safe_cast<EModifier_AF  *>(E_Angry[i]->GetModifier(MOD_AF));
          //modGrav=safe_cast<EModifier_Grav*>(E_Angry[i]->GetModifier(MOD_GRAV));
          modPose=safe_cast<EModifier_Pose*>(E_Angry[i]->GetModifier(MOD_POSE));
          modRand=safe_cast<EModifier_Rand*>(E_Angry[i]->GetModifier(MOD_RAND));

          //Set amplitude data
          modAF->SetFrequency(1.25f);
          modAF->SetAmplitude(-0.45f);
          modAF->SetFreqMix(false);

          //Set Pose Data
          modPose->SetInfluenceBlend(0.6f);
          
          //Set Random Data
          modRand->SetDuration(2.5f,4.2f);
          modRand->SetPercentRandom(0.4f);
          modRand->SetVariance(0.2f);  


      //Init Calm
      E_Calm[i]->LoadModifier(MOD_POSE,1.00f,"Emotions","Calm","Human"); 
      E_Calm[i]->LoadModifier(MOD_AF  ,1.00f,"Emotions","Calm","Human"); 
      E_Calm[i]->LoadModifier(MOD_RAND  ,1.00f,"Emotions","Calm","Human"); 
      //E_Calm[i]->LoadModifier(MOD_GRAV  ,0.50f,"Emotions","Calm","Human"); 

          //Manually set data (hacky)
          modAF  =safe_cast<EModifier_AF  *>(E_Calm[i]->GetModifier(MOD_AF));
          //modGrav=safe_cast<EModifier_Grav*>(E_Calm[i]->GetModifier(MOD_GRAV));
          modPose=safe_cast<EModifier_Pose*>(E_Calm[i]->GetModifier(MOD_POSE));
          modRand=safe_cast<EModifier_Rand*>(E_Calm[i]->GetModifier(MOD_RAND));

          //Set amplitude data
          modAF->SetFrequency(0.75f);
          modAF->SetAmplitude(0.5f);
          modAF->SetFreqMix(true);

          //Set Pose Data
          modPose->SetInfluenceBlend(1.0f);
          
          //Set Random Data
          modRand->SetDuration(2.7f,3.9f);
          modRand->SetPercentRandom(0.6f);
          modRand->SetVariance(0.3f);  


      //Init Depressed
      E_Depressed[i]->LoadModifier(MOD_POSE,1.00f,"Emotions","Depressed","Human"); 
      E_Depressed[i]->LoadModifier(MOD_AF  ,1.00f,"Emotions","Depressed","Human"); 
      E_Depressed[i]->LoadModifier(MOD_RAND,0.75f,"Emotions","Depressed","Human"); 
      //E_Depressed[i]->LoadModifier(MOD_GRAV  ,0.50f,"Emotions","Depressed","Human"); 

          //Manually set data (hacky)
          modAF  =safe_cast<EModifier_AF  *>(E_Depressed[i]->GetModifier(MOD_AF));
          //modGrav=safe_cast<EModifier_Grav*>(E_Depressed[i]->GetModifier(MOD_GRAV));
          modPose=safe_cast<EModifier_Pose*>(E_Depressed[i]->GetModifier(MOD_POSE));
          modRand=safe_cast<EModifier_Rand*>(E_Depressed[i]->GetModifier(MOD_RAND));

          //Set amplitude data
          modAF->SetFrequency(0.7f);
          modAF->SetAmplitude(0.7f);
          modAF->SetFreqMix(true);

          //Set Pose Data
          modPose->SetInfluenceBlend(1.0f);
          
          //Set Random Data
          modRand->SetDuration(3.5f,4.2f);
          modRand->SetPercentRandom(0.1f);
          modRand->SetVariance(0.2f);  


      //Init Happy
      E_Happy[i]->LoadModifier(MOD_POSE,1.00f,"Emotions","Happy","Human"); 
      E_Happy[i]->LoadModifier(MOD_AF  ,1.00f,"Emotions","Happy","Human"); 
      E_Happy[i]->LoadModifier(MOD_RAND  ,0.75f,"Emotions","Happy","Human"); 
      //E_Happy[i]->LoadModifier(MOD_GRAV  ,0.50f,"Emotions","Happy","Human"); 

          //Manually set data (hacky)
          modAF  =safe_cast<EModifier_AF  *>(E_Happy[i]->GetModifier(MOD_AF));
          //modGrav=safe_cast<EModifier_Grav*>(E_Happy[i]->GetModifier(MOD_GRAV));
          modPose=safe_cast<EModifier_Pose*>(E_Happy[i]->GetModifier(MOD_POSE));
          modRand=safe_cast<EModifier_Rand*>(E_Happy[i]->GetModifier(MOD_RAND));

          //Set amplitude data
          modAF->SetFrequency(0.8f);
          modAF->SetAmplitude(-0.2f);
          modAF->SetFreqMix(true);

          //Set Pose Data
          modPose->SetInfluenceBlend(1.0f);
          
          //Set Random Data
          modRand->SetDuration(4.5f,5.2f);
          modRand->SetPercentRandom(0.2f);
          modRand->SetVariance(0.3f);  


      //Assign the target emotion
      ModifyEmotionCharacter(i,randG.NextFloat(emotionMin,emotionMax),1.0f/*randG.NextFloat(0.8f,1.0f)*/);  

  }
  

 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: ModifyAll
PURPOSE  : To modify all characters to have a emotion target within 
           variance of emoTarget
PRE : emoTarget is between 0 and 360, 0 < variance < 180 
POST: All characters now have emotions winthin 90 of emoTarget and 
      have emoInfTarget influence.
-------------------------------------------------------------------*/
void VEInferenceEngine::ModifyAll(float emoTarget,float emoInfTarget,float variance){

unsigned int i;

  //Set the global parameters
  globalTarget=emoTarget;
  globalInfluence=emoInfTarget;

  
  //Loop for all characters and reset their parameters
  for(i=0;i<emotionTarget.Count();i++){
       
    //Re-assign for new global parameters
    ModifyEmotionCharacter(i,emotionTarget[i],emotionInfTarget[i]);    
  }


 return;
}

/*-------------------------------------------------------------------
PROCEDURE: ModifyEmotionCharacter
PURPOSE  : To modify a character to the exact emotion target and 
           influence.
PRE : There exists a character for the index, emoTarget is between 
      0 and 360.
POST: The character has had it's emotions modified.
-------------------------------------------------------------------*/
void VEInferenceEngine::ModifyEmotionCharacter(unsigned int index,float emoTarget,float emoInfTarget){
float angry,happy,depressed,calm;
float moveSpeed;

  calm=0.0f;
  angry=0.0f;
  happy=0.0f;
  depressed=0.0f;

  //Clamp the target and influence
  if(emoTarget >= 360.0f){ emoTarget =360.0f;}
  if(emoTarget <    0.0f){ emoTarget =  0.0f;}
  if(emoInfTarget > 1.0f){ emoInfTarget =1.0f;}
  if(emoInfTarget < 0.0f){ emoInfTarget =0.0f;}

  //Assign the values
  emotionTarget[index]   =emoTarget;
  emotionInfTarget[index]=emoInfTarget;

  //Scale the emotion target by the global value
  if(globalTarget < emoTarget){
    if(emoTarget-globalTarget > 180.0f){
      emoTarget=emoTarget+((globalTarget-emoTarget+360.0f) * globalInfluence);  
    }else{
      emoTarget=globalTarget+((emoTarget-globalTarget) * (1.0f-globalInfluence));  
    }
  }else{
    if(globalTarget-emoTarget < 180.0f){
      emoTarget=emoTarget+((globalTarget-emoTarget) * globalInfluence);
    }else{
      emoTarget=globalTarget+((emoTarget-globalTarget+360.0f) * (1.0f-globalInfluence));
    }
  }

  //Clamp again
  if(emoTarget <   0.0f){ emoTarget +=360.0f;}
  if(emoTarget > 360.0f){ emoTarget -=360.0f;}

  //Decide what two emotions the target is between (hacky- better way of doing this)
  if(emoTarget >= 0.0f && emoTarget < 90.0f){
    //Between Happy and Angry
    angry=emoTarget/90.0f;
    happy=(90.0f-emoTarget)/90.0f;
    
    //Determine speed
    moveSpeed=emotionSpeeds[0]*angry+emotionSpeeds[3]*happy;

    //Scale by emotion influence
    happy*=emoInfTarget;
    angry*=emoInfTarget;

  }else
  if(emoTarget >= 90.0f && emoTarget < 225.0f){
    //Between Angry and Depressed
    depressed=(emoTarget-90.0f)/135.0f;
    angry=(225.0f-emoTarget)/135.0f;

    //Determine speed
    moveSpeed=emotionSpeeds[0]*angry+emotionSpeeds[2]*depressed;

    //Scale by emotion influence
    angry*=emoInfTarget;
    depressed*=emoInfTarget;


  }else
  if(emoTarget >= 225.0f && emoTarget < 315.0f){
    //Between Depressed and Calm
    calm=(emoTarget-225.0f)/90.0f;
    depressed=(315.0f-emoTarget)/90.0f;

    //Determine speed
    moveSpeed=emotionSpeeds[1]*calm+emotionSpeeds[2]*depressed;

    //Scale by emotion influence
    depressed*=emoInfTarget;
    calm*=emoInfTarget;


  }else
  if(emoTarget >= 315.0f && emoTarget < 360.0f){
    //Between calm and Happy
    happy=(emoTarget-315.0f)/45.0f;
    calm=(360.0f-emoTarget)/45.0f;

    //Determine speed
    moveSpeed=emotionSpeeds[1]*calm+emotionSpeeds[3]*happy;

    //Scale by emotion influence
    calm*=emoInfTarget;
    happy*=emoInfTarget;
    //LOGWARN(("Calm: %f. Happy: %f Degree: %f",calm,happy,emoTarget));
  }

  //Assign the emotions
  E_Angry    [index]->SetModifierInfluence(MOD_ALL,angry);
  E_Calm     [index]->SetModifierInfluence(MOD_ALL,calm);
  E_Depressed[index]->SetModifierInfluence(MOD_ALL,depressed);
  E_Happy    [index]->SetModifierInfluence(MOD_ALL,happy);

  //E_Angry    [index]->SetModifierInfluence(MOD_ALL,0.0f);
  //E_Depressed[index]->SetModifierInfluence(MOD_ALL,depressed);
  
  //Set the character movement speed (scaled by the influnce away from the default speed of 4.0f)
  (*emoC)[index]->SetSpeed(moveSpeed*emoInfTarget + (1.0f-emoInfTarget) *4.0f);  
  
  //LOGWARN(("MoveSpeed: %f.",moveSpeed));
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: GetEmotionTarget
PURPOSE  : To return the the current emotion target.
PRE : There exists a character for the index.
POST: The current emotion target is returned.
-------------------------------------------------------------------*/
float VEInferenceEngine::GetEmotionTarget(unsigned int index){

 return emotionTarget[index];
}

/*-------------------------------------------------------------------
PROCEDURE: GetEmotionInfTarget
PURPOSE  : To return the influence of the current emotion
PRE : There exists a character for the index.
POST: The current influence of the current emotion is returned.
-------------------------------------------------------------------*/
float VEInferenceEngine::GetEmotionInfTarget(unsigned int index){

 return emotionInfTarget[index];
}


/*-------------------------------------------------------------------
PROCEDURE: GetGlobalTarget
PURPOSE  : To return the global target.
PRE : There exists a character for the index.
POST: The current global target is returned.
-------------------------------------------------------------------*/
float VEInferenceEngine::GetGlobalTarget(){

 return globalTarget;
}

/*-------------------------------------------------------------------
PROCEDURE: GetGlobalInfluence
PURPOSE  : To return the global influence.
PRE : There exists a character for the index.
POST: The current global influence is returned.
-------------------------------------------------------------------*/
float VEInferenceEngine::GetGlobalInfluence(){

 return globalInfluence;
}