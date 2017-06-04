/*--------------------------------------------------------------------
  EModifier_Rand.h

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on making random movements to skeleton bones.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_EMODIFIER_RAND_H_
#define _INCLUDED_EMODIFIER_RAND_H_

#include <File.h>
#include <Rand.h>
#include <PString.h>
#include <Vector3.h>   
#include <Modifier.h>
#include <Quaternion.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>

#include "EModifier.h"
#include "EmotionModifier.h"

class EModifier; 

using namespace Jet;

class EModifier_Rand : public EModifier  
{
public:
    EModifier_Rand();
    virtual ~EModifier_Rand();

    virtual Modifier::Result Update(float dt);
    virtual bool LoadData(EmotionModifier * em,float emotionInfluence, 
                          PString realPath,PString objectLoad);

    void SetPercentRandom(float r);
    void SetVariance(float v);
    void SetDuration(float min,float max);
    void SetInfluenceBlend(float ib);
private:
    typedef FastDynamicArray<float> FloatArray;
    typedef DynamicArray<Vector3,116/sizeof(Vector3)> Vec3Array;

    void AssignRandomBones(void);

    //The maximum percentage of the bones that will be moving at any one time
    float percentRandom;

    //How far the bones will move
    float variance;

    //The duration of randomness (rads/sec)
    float minDuration,maxDuration;

    //The random generator
    Rand rand;

    //The duration of each bone
    FloatArray boneDuration,boneTime;
    Vec3Array  boneAdd;

    unsigned int numMoving,maxMoving,numBones;

    ParticlePtrArray * pBones;
    float influenceBlend;
};

#endif 
