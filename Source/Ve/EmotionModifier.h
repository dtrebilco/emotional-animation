/*--------------------------------------------------------------------
  EmotionModifier.h

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on specific emotion data.

  NOTE: This is based on a Modifier but not exactly as it can only be 
        used in specific circumstances. (ie only registered with a 
        EmotionSkeletonModifier)

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_EMOTION_MODIFIER_H_
#define _INCLUDED_EMOTION_MODIFIER_H_

#include <File.h>
#include <assert.h>
#include <PString.h>
#include <Vector3.h>   
#include <Modifier.h>
#include <Quaternion.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>
#include <SkeletonModifier.h>

#include "SkeletonFile.h"
#include "EmotionSkeletonModifier.h"

typedef FastDynamicArray<Particle *> ParticlePtrArray;

#include "EModifier.h"
#include "EModifier_AF.h"
#include "EModifier_Pose.h"
#include "EModifier_Grav.h"
#include "EModifier_Rand.h"

using namespace Jet;

class EModifier; 
class EmotionSkeletonModifier;

enum E_MODIFIER_FLAG{
    MOD_MIN  = 0, 

    MOD_AF   = 0,
    MOD_POSE = 1,
    MOD_GRAV = 2,
    MOD_RAND = 3,
    
    MOD_ALL  = 5,
    MOD_MAX  = 5,
};

class EmotionModifier  
{
public:
	EmotionModifier(EmotionSkeletonModifier *host);
	virtual ~EmotionModifier();
   
    //Methods similar to Modifier class
    virtual Modifier::Result Update(float dt);


    //New methods
    int LoadEmotionDB(float emotionInfluence,PString path,PString emotionName,
                       PString ObjectLoad=PString(""));
  
    int LoadModifier(E_MODIFIER_FLAG eMod,float emotionInfluence,PString path,PString emotionName,
                       PString ObjectLoad=PString(""));

    int UnLoadModifier(E_MODIFIER_FLAG eMod);
    int SetModifierInfluence(E_MODIFIER_FLAG eMod,float emotionInfluence);

    ParticleStructure * GetParticleStruct();
    ParticlePtrArray  * GetBoneStruct();
    EmotionSkeletonModifier * GetHost();
    EModifier * GetModifier(E_MODIFIER_FLAG eMod);
    
private:
    
    virtual Modifier::Result SetHost(EmotionSkeletonModifier *host);
    void GetBones(Particle *p);

    //Array to store pointers to the bone particles
    ParticlePtrArray pArray;

    //Host for this emotion modifier 
    EmotionSkeletonModifier *host;

    //The modifiers
    EModifier * eModifiers[MOD_MAX];

};

#endif 
