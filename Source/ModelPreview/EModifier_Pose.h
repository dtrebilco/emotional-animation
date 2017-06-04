/*--------------------------------------------------------------------
  EModifier_Pose.h

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on pre-defined emotion positions.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_EMODIFIER_POSE_H_
#define _INCLUDED_EMODIFIER_POSE_H_

#include <File.h>
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

class EModifier_Pose : public EModifier  
{
public:
	EModifier_Pose();
	virtual ~EModifier_Pose();

    virtual Modifier::Result Update(float dt);
    virtual bool LoadData(EmotionModifier * em,float emotionInfluence, 
                          PString realPath,PString objectLoad);
    void SetInfluenceBlend(float ib);
private:

    typedef DynamicArray<Quaternion,116/sizeof(Quaternion)> QuatArray;
    typedef DynamicArray<Vector3,116/sizeof(Vector3)> Vec3Array;
    typedef FastDynamicArray<float> FloatArray;

    unsigned int LoadNeutralSkeleton(PString fileName);
    unsigned int LoadPoseSkeleton   (PString fileName);

    QuatArray poseOrients;
    Vec3Array posePositions;
    FloatArray positionInf,orientationInf;
   
    ParticlePtrArray * pBones;
 
    float influenceBlend;
    float positionInfLimit; 
    float orientationInfLimit; 
};

#endif 
