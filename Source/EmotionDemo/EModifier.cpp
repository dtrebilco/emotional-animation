/*--------------------------------------------------------------------
  EModifier.cpp

  Contains the base class for all emotion modifiers.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "stdafx.h"
#include "EModifier.h"

using namespace Jet;

LOGCLIENT(EModifier);


/*-------------------------------------------------------------------
PROCEDURE: EModifier
PURPOSE  : To construct the object using default parameters.
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
EModifier::EModifier():
init(false),
influence(0.0f)
{

}

/*-------------------------------------------------------------------
PROCEDURE: ~EModifier
PURPOSE  : To destruct the object. 
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
EModifier::~EModifier()
{

}

/*-------------------------------------------------------------------
PROCEDURE: GetInfluence
PURPOSE  : To get the influence of this object. 
PRE : TRUE
POST: The influence is returned.
-------------------------------------------------------------------*/
float EModifier::GetInfluence(){
 return influence;
}

/*-------------------------------------------------------------------
PROCEDURE: SetInfluence
PURPOSE  : To set the influence of this object. 
PRE : TRUE
POST: The influence is set. (Influences that are negative and are 
      > 1.0 are acceptable but may produce strange results)
-------------------------------------------------------------------*/
void EModifier::SetInfluence(float inf){
  influence=inf;
}

/*-------------------------------------------------------------------
PROCEDURE: IsInit
PURPOSE  : To get the init status of this modifier. 
PRE : TRUE
POST: The init status is returned.
-------------------------------------------------------------------*/
bool EModifier::IsInit(){
 return init;
}


/*-------------------------------------------------------------------
PROCEDURE: GetBoneData
PURPOSE  : To calculate bone data (ie min,max, average) positions 
           and orientations that occur for the given modifiers.
PRE : mf,boneStruct,posOrien are valid pointers. (ie not NULL) and 
      the size of boneStruct equals posOrien.
POST: The calculated bone positions are returned in posOrien. 
-------------------------------------------------------------------*/
void EModifier::GetBoneData(ModifierArray * mf,ParticlePtrArray * boneStruct,
                            PlacementArray *posOrien,unsigned int flags){
AnimationModifier *thisam=&aa;

 //First get all the animation modifiers to process 


 //For each animation, access bone data and calculate limits/average 

 //Copy data over so we can access it
 *thisam=*((AnimationModifier *)(*mf->First()));


 return;
}

EModifier::AccessAnimation::AccessAnimation():
AnimationModifier("NoName")
{
    

}