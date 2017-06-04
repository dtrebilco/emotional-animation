/*--------------------------------------------------------------------
  EmotionSkeletonModifier.h

  Contains the implementation of the class that allows the 
  loading of emotions onto a skeleton.

  02/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_EMOTION_SKELETION_MODIFIER_H_
#define _INCLUDED_EMOTION_SKELETION_MODIFIER_H_

#include <File.h>
#include <Vector3.h>   
#include <Quaternion.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>
#include <SkeletonModifier.h>

#include "SkeletonFile.h"
#include "EmotionModifier.h"

using namespace Jet;

class EmotionModifier; 

class EmotionSkeletonModifier : public SkeletonModifier  
{
public:

    //Inherited constructors
    EmotionSkeletonModifier(ParticleStructure *object, float priority = MODIFIER_DEFAULT_PRIORITY);
	EmotionSkeletonModifier(ParticleStructure *object, ModifierHost *host,float priority = MODIFIER_DEFAULT_PRIORITY);
	EmotionSkeletonModifier(ParticleStructure *object, RenderKernel *renderKernel,float priority = MODIFIER_DEFAULT_PRIORITY);

	virtual ~EmotionSkeletonModifier();

    //Overridden methods
    virtual Modifier::Result Update(float dt);

    //New methods
    void AddEmotionModifier   (EmotionModifier *em);
    bool RemoveEmotionModifier(EmotionModifier *em);
    ParticleStructure * GetParticleStruct();

    //Method to get the standard modifiers(required by a emotion modifiers)
    ModifierArray * GetModifiers();

private:
    typedef FastDynamicArray<EmotionModifier *> EmotionModifierArray;

    //The particle structure we are pointing at
    ParticleStructure * psObject;
    
    //The array of emotions
    EmotionModifierArray emotionArray;

};

#endif 
