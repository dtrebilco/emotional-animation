/*--------------------------------------------------------------------
  VEInferenceEngine.h

  Contains the virtual environment emotion inference engine.

  08/06/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_VE_INFERENCE_ENGINE_H_
#define _INCLUDED_VE_INFERENCE_ENGINE_H_

#include <File.h>
#include <PString.h>
#include <Vector3.h>   
#include <Modifier.h>
#include <Quaternion.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>
#include <AnimationModifier.h>

class EmotionModifier;
class VECharacter;
class VEInferenceEngine;

#include "EmotionModifier.h"

using namespace Jet;

typedef FastDynamicArray<VECharacter  *> *CharacterArrayPointer;

class VEInferenceEngine : public Modifier 
{
public:
	VEInferenceEngine();
	virtual ~VEInferenceEngine();

    bool Init(CharacterArrayPointer emotionC,
              float emotionMin,float emotionMax);
    virtual Modifier::Result Update(float dt);
    
    void ModifyAll(float emoTarget,float emoInfTarget,float variance);
    void ModifyEmotionCharacter(unsigned int index,float emoTarget,float emoInfTarget);

    float GetEmotionTarget(unsigned int index);
    float GetEmotionInfTarget(unsigned int index);
    float GetGlobalTarget();
    float GetGlobalInfluence();

private:
    typedef FastDynamicArray<VECharacter  *> *CharacterArrayPointer;
    typedef FastDynamicArray<EmotionModifier  *> EmotionModifierArray;
    typedef FastDynamicArray<float> FloatArray;

    //The emotion characters
    CharacterArrayPointer emoC;

    //The emotions for each character
    EmotionModifierArray E_Angry;
    EmotionModifierArray E_Depressed;
    EmotionModifierArray E_Happy;
    EmotionModifierArray E_Calm;

    //The target and influence of the emotions
    FloatArray emotionTarget;
    FloatArray emotionInfTarget;
    float globalTarget,globalInfluence;

    //The movement speed for each emotion
    float emotionSpeeds[4];

};

//Have to put this here for dependicies
#include "ve.h"

#endif 
