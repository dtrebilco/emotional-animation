/*--------------------------------------------------------------------
  EModifier.h

  Contains the base class for all emotion modifiers.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_EMODIFIER_H_
#define _INCLUDED_EMODIFIER_H_

#include <File.h>
#include <PString.h>
#include <Vector3.h>   
#include <Modifier.h>
#include <Quaternion.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>
#include <AnimationModifier.h>

class EmotionModifier;

using namespace Jet;
typedef FastDynamicArray<Particle *> ParticlePtrArray;
typedef DynamicArray<Placement,116/sizeof(Placement)> PlacementArray;

class EModifier   
{
public:
    EModifier();
    virtual ~EModifier();

    virtual Modifier::Result Update(float dt)=0;

    virtual bool LoadData(EmotionModifier * em,float emotionInfluence, 
                          PString realPath,PString objectLoad)=0;

    float GetInfluence();
    virtual void  SetInfluence(float inf);

    bool IsInit();

protected:

    void GetBoneData(ModifierArray * mf,ParticlePtrArray * boneStruct,
                     PlacementArray *posOrien,unsigned int flags);

    //The influence factor of this modifier
    float influence;

    //If the modifier is init.
    bool init;

private:
    class AccessAnimation :public AnimationModifier{
       friend class EModifier;
       AccessAnimation();  
    };
    AccessAnimation aa;
};

#endif 
