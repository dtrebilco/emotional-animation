/*--------------------------------------------------------------------
  EModifier_Grav.h

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on a gravity direction vector.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_EMODIFIER_GRAV_H_
#define _INCLUDED_EMODIFIER_GRAV_H_
#include <math.h>

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

class EModifier_Grav : public EModifier  
{
public:
    EModifier_Grav();
    virtual ~EModifier_Grav();

    virtual Modifier::Result Update(float dt);
    virtual bool LoadData(EmotionModifier * em,float emotionInfluence, 
                          PString realPath,PString objectLoad);
    
    void SetGravPosition(Vector3 pos);
    void SetInfluenceBlend(float ib);

private:
    typedef DynamicArray<Quaternion,116/sizeof(Quaternion)> QuatArray;
    typedef DynamicArray<Vector3,116/sizeof(Vector3)> Vec3Array;
    typedef FastDynamicArray<float> FloatArray;

    QuatArray  gravOrients;
    Vec3Array  gravPositions;
    FloatArray distArray;

    Vector3 gravPos;
    ParticlePtrArray * pBones;

    float influenceBlend;
    //Quaternion gravOrientation;

};

#endif 
