//
// EmotionObject.cpp
//
// Contains the implementation for an example object containing a mesh.
//

#include "StdAfx.h"
#include "EmotionObject.h"
// Render support header files
#include <IndexedMeshInstance.h>
#include <GeometryBaseInstance.h>
#include <ScalableDetail.h>


//
// All Jet classes are encapsulated in the 'Jet' namespace.  Before any Jet objects
// are used, the following namespace must be imported.
//
using namespace Jet;



//
// Define the logging client name for this example
//
LOGCLIENT(EmotionObject);



///////////////////////////////////////////////////////////////////////////////
// Mesh object implementation

//
// Constructor
//
EmotionObject::EmotionObject():
skeleton(NULL), 
animator(NULL),
testEmotion(NULL),
currEmotionNum(0),
oldtestEmotion(NULL)
{
}

//
// Destructor
//
EmotionObject::~EmotionObject(){

  if(testEmotion){DeleteMemZ(testEmotion);}
  if(oldtestEmotion){DeleteMemZ(oldtestEmotion);}
  DeleteMemZ(animator);
  DeleteMemZ(skeleton);
}


//
// Load object and initialize it
//
bool EmotionObject::Init(RenderKernel *renderKernel, ResourceDatabase *database, 
                        const PString &name)
{
  this->renderKernel = renderKernel;

  //
  // Mesh information is loaded using the geometry loader.  This must be initialized
  // with the resource database and name of the file before use.
  //
  GeometryBaseInstance geometry;
  geometry.Init(database, name);

  if(!geometry.CreateInstance(renderKernel, this)){
    return false;
  }

  // Create a skeleton modifier from the loaded mesh so that movement 
  // and animation can blend together
  skeleton = NewMem(EmotionSkeletonModifier(this, renderKernel));

  // Create an animation interface for the movement system to use
  animator = NewMem(Animator(database, skeleton));
  
 
  LoadAnimation(database,"idle_normal.kin");

  return true;
}


/*-------------------------------------------------------------------
PROCEDURE: LoadAnimation
PURPOSE  : To load a animation from a database. 
PRE : TRUE
POST: If a animation could be loaded returns true
      else returns false
-------------------------------------------------------------------*/
bool EmotionObject::LoadAnimation(ResourceDatabase *database,const PString &name){


  //Load the animation onto the root bone (and remove previous animations)
  animator->RemoveAll();

  //Try to play the base animation
  if(animator->Add(NULL, name, AnimationModifier::CYCLIC | AnimationModifier::ROOT_HORIZ_OFF,
                   1.0f, 0.0f, 1.0f)!=OK){
    return false;
  }
 
  LoadEmotion(currEmotionNum);
  SetInfluence(1.0f,0.0f);

 return true;
}

bool EmotionObject::LoadEmotion(unsigned int num){
EModifier_AF   * modAF;
EModifier_Pose * modPose;
EModifier_Rand * modRand;
EModifier_Grav * modGrav;

  //Delete old existing emotion
  if(oldtestEmotion){
    DeleteMemZ(oldtestEmotion); 
  }
  oldtestEmotion=testEmotion; 

  //Load a new one
  testEmotion=NewMem(EmotionModifier(skeleton));

  //Load specific modifiers
  switch(num){
    case(0): break;
    case(1): //Load Angry
             testEmotion->LoadModifier(MOD_POSE,1.00f,"Emotions","Angry","Human"); 
             testEmotion->LoadModifier(MOD_AF  ,1.00f,"Emotions","Angry","Human"); 
             testEmotion->LoadModifier(MOD_RAND,1.00f,"Emotions","Angry","Human"); 

              //Manually set data (hacky)
              modAF  =safe_cast<EModifier_AF  *>(testEmotion->GetModifier(MOD_AF));
              modPose=safe_cast<EModifier_Pose*>(testEmotion->GetModifier(MOD_POSE));
              modRand=safe_cast<EModifier_Rand*>(testEmotion->GetModifier(MOD_RAND));

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

             break;

    case(2): //Load Calm
              testEmotion->LoadModifier(MOD_POSE,1.00f,"Emotions","Calm","Human"); 
              testEmotion->LoadModifier(MOD_AF  ,1.00f,"Emotions","Calm","Human"); 
              testEmotion->LoadModifier(MOD_RAND,1.00f,"Emotions","Calm","Human"); 

              //Manually set data (hacky)
              modAF  =safe_cast<EModifier_AF  *>(testEmotion->GetModifier(MOD_AF));
              modPose=safe_cast<EModifier_Pose*>(testEmotion->GetModifier(MOD_POSE));
              modRand=safe_cast<EModifier_Rand*>(testEmotion->GetModifier(MOD_RAND));

              //Set amplitude data
              modAF->SetFrequency(0.75f);
              modAF->SetAmplitude(0.5f);
              modAF->SetFreqMix(false);

              //Set Pose Data
              modPose->SetInfluenceBlend(1.0f);
          
              //Set Random Data
              modRand->SetDuration(2.7f,3.9f);
              modRand->SetPercentRandom(0.6f);
              modRand->SetVariance(0.3f);  
        
             break;

    case(3): //Load Depressed
              //Init Depressed
              testEmotion->LoadModifier(MOD_POSE,1.00f,"Emotions","Depressed","Human"); 
              testEmotion->LoadModifier(MOD_AF  ,1.00f,"Emotions","Depressed","Human"); 
              testEmotion->LoadModifier(MOD_RAND,1.00f,"Emotions","Depressed","Human"); 
              

                  //Manually set data (hacky)
                  modAF  =safe_cast<EModifier_AF  *>(testEmotion->GetModifier(MOD_AF));
                  modPose=safe_cast<EModifier_Pose*>(testEmotion->GetModifier(MOD_POSE));
                  modRand=safe_cast<EModifier_Rand*>(testEmotion->GetModifier(MOD_RAND));

                  //Set amplitude data
                  modAF->SetFrequency(0.7f);
                  modAF->SetAmplitude(0.7f);
                  modAF->SetFreqMix(false);

                  //Set Pose Data
                  modPose->SetInfluenceBlend(1.0f);
          
                  //Set Random Data
                  modRand->SetDuration(3.5f,4.2f);
                  modRand->SetPercentRandom(0.1f);
                  modRand->SetVariance(0.2f);  

             break;


    case(4): //Load Happy
              //Init Happy
              testEmotion->LoadModifier(MOD_POSE,1.00f,"Emotions","Happy","Human"); 
              testEmotion->LoadModifier(MOD_AF  ,1.00f,"Emotions","Happy","Human"); 
              testEmotion->LoadModifier(MOD_RAND,1.00f,"Emotions","Happy","Human"); 

                  //Manually set data (hacky)
                  modAF  =safe_cast<EModifier_AF  *>(testEmotion->GetModifier(MOD_AF));
                  modPose=safe_cast<EModifier_Pose*>(testEmotion->GetModifier(MOD_POSE));
                  modRand=safe_cast<EModifier_Rand*>(testEmotion->GetModifier(MOD_RAND));

                  //Set amplitude data
                  modAF->SetFrequency(0.8f);
                  modAF->SetAmplitude(-0.2f);
                  modAF->SetFreqMix(false);

                  //Set Pose Data
                  modPose->SetInfluenceBlend(1.0f);
          
                  //Set Random Data
                  modRand->SetDuration(4.5f,5.2f);
                  modRand->SetPercentRandom(0.2f);
                  modRand->SetVariance(0.3f);  
        
             break;
    case(5): 
              //Init Walk in wind
              testEmotion->LoadModifier(MOD_GRAV,1.00f,"Emotions","Happy","Human"); 

                  //Manually set data (hacky)
                  modGrav=safe_cast<EModifier_Grav*>(testEmotion->GetModifier(MOD_GRAV));

                  modGrav->SetGravPosition(Vector3(0.0f,-10.0f,5.0f));          
                  modGrav->SetInfluenceBlend(0.6f); 
        
             break;
    case(6):  //Skitzoid
              testEmotion->LoadModifier(MOD_RAND,1.00f,"Emotions","Happy","Human"); 

                  //Manually set data (hacky)
                  modRand=safe_cast<EModifier_Rand*>(testEmotion->GetModifier(MOD_RAND));
         
                  //Set Random Data
                  modRand->SetDuration(0.02f,0.10f);
                  modRand->SetPercentRandom(0.6f);
                  modRand->SetVariance(0.05f);
                  modRand->SetInfluenceBlend(0.7f); 
        
             break;
    case(7):      //Drunk 
                  testEmotion->LoadModifier(MOD_RAND,1.00f,"Emotions","Happy","Human"); 
                  testEmotion->LoadModifier(MOD_AF  ,1.00f,"Emotions","Happy","Human"); 

                  //Manually set data (hacky)
                  modRand=safe_cast<EModifier_Rand*>(testEmotion->GetModifier(MOD_RAND));
                  modAF  =safe_cast<EModifier_AF  *>(testEmotion->GetModifier(MOD_AF));
       
                  //Set amplitude data
                  modAF->SetFrequency(0.5f);
                  modAF->SetAmplitude(0.2f);
                  modAF->SetFreqMix(false);

                  //Set Random Data
                  modRand->SetDuration(4.02f,6.10f);
                  modRand->SetPercentRandom(0.5f);
                  modRand->SetVariance(0.65f);
                  modRand->SetInfluenceBlend(0.7f); 

              break;

    case(8):  //Scrurry
                  //testEmotion->LoadModifier(MOD_RAND,1.00f,"Emotions","Happy","Human"); 
                  testEmotion->LoadModifier(MOD_AF  ,1.00f,"Emotions","Happy","Human"); 

                  //Manually set data (hacky)
                  //modRand=safe_cast<EModifier_Rand*>(testEmotion->GetModifier(MOD_RAND));
                  modAF  =safe_cast<EModifier_AF  *>(testEmotion->GetModifier(MOD_AF));
       
                  //Set amplitude data
                  modAF->SetFrequency(3.5f);
                  modAF->SetAmplitude(0.7f);
                  modAF->SetFreqMix(false);

                  //Set Random Data
                  //modRand->SetDuration(4.02f,6.10f);
                  //modRand->SetPercentRandom(0.5f);
                  //modRand->SetVariance(0.65f);
                  //modRand->SetInfluenceBlend(0.7f); 

             break;

    default: return false;
  }
  currEmotionNum=num;

 return true;
}


//Set the influences
void EmotionObject::SetInfluence(float newInf,float oldInf){

  if(oldtestEmotion){
    oldtestEmotion->SetModifierInfluence(MOD_ALL,oldInf);   
  }
  if(testEmotion){
    testEmotion->SetModifierInfluence(MOD_ALL,newInf);   
  }
}
