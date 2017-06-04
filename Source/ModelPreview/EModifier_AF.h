/*--------------------------------------------------------------------
  EModifier_AF.h

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on amplitude and frequency movements.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_EMODIFIER_AF_H_
#define _INCLUDED_EMODIFIER_AF_H_

#include <File.h>
#include <PString.h>
#include <Vector3.h>   
#include <Modifier.h>
#include <Quaternion.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>

#include "EModifier.h"
#include "EMotionModifier.h"

class EModifier; 

using namespace Jet;

class EModifier_AF : public EModifier 
{
public:
    EModifier_AF();
    virtual ~EModifier_AF();

    virtual Modifier::Result Update(float dt);
    virtual bool LoadData(EmotionModifier * em,float emotionInfluence, 
                          PString realPath,PString objectLoad);

    void SetInfluence(float inf );
    void SetFrequency(float freq);
    void SetAmplitude(float amp );
    void SetFreqMix  (bool  mix );

    //TODO: Get methods

private:
    typedef DynamicArray<Quaternion,116/sizeof(Quaternion)> QuatArray;
    typedef DynamicArray<Vector3,116/sizeof(Vector3)> Vec3Array;
    typedef FastDynamicArray<float> FloatArray;

    unsigned int LoadNeutralSkeleton(PString fileName);

    QuatArray neutralOrients;
    Vec3Array neutralPositions;
    FloatArray neutralMagnitudes;

    ParticlePtrArray * pBones;

    //The whole frequency and amplitude values
    float freqInfluence,ampInfluence;

    //The scaled frequency and amplitude values
    float currFreqInf, currAmpInf;

    //The parent emotion modifier (needed for altering frequency values)
    EmotionModifier *emotionMod;

    //Flag to indicate weither or not to mix the frequency with the current animation speed
    bool freqMix;
};

#endif 
