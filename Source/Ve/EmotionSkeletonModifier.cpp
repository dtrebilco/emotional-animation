/*--------------------------------------------------------------------
  EmotionSkeletonModifier.h

  Contains the implementation of the class that allows the 
  loading of emotions onto a skeleton.

  02/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "stdafx.h"
#include "EmotionSkeletonModifier.h"

using namespace Jet;

LOGCLIENT(EmotionSkeletonModifier);

/*-------------------------------------------------------------------
PROCEDURE: EmotionSkeletonModifier
PURPOSE  : Build list of Bone Blenders from given object and set 
           default parameters
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
EmotionSkeletonModifier::EmotionSkeletonModifier(ParticleStructure *object, 
                                                 float priority):
SkeletonModifier(object,priority),
psObject(object)
{


}

/*-------------------------------------------------------------------
PROCEDURE: EmotionSkeletonModifier
PURPOSE  : Build list of Bone Blenders from given object, set default
           parameters and add modifier to the given host.
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
EmotionSkeletonModifier::EmotionSkeletonModifier(ParticleStructure *object,
                                                 ModifierHost *host,
                                                 float priority):
SkeletonModifier(object,host,priority),
psObject(object)
{


}

/*-------------------------------------------------------------------
PROCEDURE: EmotionSkeletonModifier
PURPOSE  : Build list of Bone Blenders from given object, set default
           parameters and add modifier to the default host.
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
EmotionSkeletonModifier::EmotionSkeletonModifier(ParticleStructure *object,
                                                 RenderKernel *renderKernel,
                                                 float priority):
SkeletonModifier(object,renderKernel,priority),
psObject(object)
{



}


/*-------------------------------------------------------------------
PROCEDURE: ~EmotionSkeletonModifier
PURPOSE  : To destruct the object. Remove this modifier from the host,
           clear and destroy all bone blenders, and clear all bone 
           modifier lists.(but do not destroy),
           
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
EmotionSkeletonModifier::~EmotionSkeletonModifier(){

  //Just report that there are remaining emotion modifiers
  if(!emotionArray.IsEmpty()){
     LOGWARN(("~EmotionSkeletonModifier: Emotion Modifiers still existing."));    
  }
    
/*    
  EmotionModifierArray::Iterator em;
  //Remove all remaining emotion modifiers
  for (em=emotionArray.First();em!=emotionArray.End();em++){

    //Delete emotion modifier
    DeleteMem(*em);
  }
*/

}


/*-------------------------------------------------------------------
PROCEDURE: Update
PURPOSE  : Perform any required changes to the object this frame.
PRE : TRUE
POST: Returns OK if no problems were encountered.
      Modifier::RES_REMOVE  if the host should remove this modifier.
      Modifier::RES_REMOVE_DESTROY if host should remove and destroy
      this modifier.
-------------------------------------------------------------------*/
Modifier::Result EmotionSkeletonModifier::Update(float dt){
Modifier::Result result(OK);


  //Call parent "Update" method
  result=SkeletonModifier::Update(dt); 

  //Only if parent returned "OK" perform our emotion modifications
  if(result!=OK){
    return result;
  }
  

  //test loading of skeleton..
  //SkeletonFile::LoadSkeleton(psObject,"test.ske"); 


  EmotionModifierArray::Iterator em;
  //Perform update on all emotion modifiers
  for (em=emotionArray.First();em!=emotionArray.End();em++){

    //Perform update
    em->Update(dt); 
  }

 return OK;
}

/*-------------------------------------------------------------------
PROCEDURE: AddEmotionModifier
PURPOSE  : To add the passed emotion modifier to the current list of 
           emotion modifiers. (If it does not already exist on the 
           list) 
PRE : TRUE
POST: The passed emotion modifier has been added.
-------------------------------------------------------------------*/
void EmotionSkeletonModifier::AddEmotionModifier(EmotionModifier *em){
  emotionArray.AddUnique(em);
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: RemoveEmotionModifier
PURPOSE  : To remove the passed emotion modifier. 
           (But not deleting it)
PRE : TRUE
POST: The passed emotion modifier has been removed from the list of 
      emotion modifiers. (But not deleted)
-------------------------------------------------------------------*/
bool EmotionSkeletonModifier::RemoveEmotionModifier(EmotionModifier *em){
  return emotionArray.FindRemove(em); 
}


/*-------------------------------------------------------------------
PROCEDURE: GetParticleStruct
PURPOSE  : To get the particle structure that this skeleton uses.
PRE : TRUE
POST: The current particle structure is returned.
-------------------------------------------------------------------*/
ParticleStructure * EmotionSkeletonModifier::GetParticleStruct(){
 return psObject;
}

/*-------------------------------------------------------------------
PROCEDURE: GetModifiers
PURPOSE  : To get the list of registered modifiers.
PRE : TRUE
POST: The current list of modifiers is returned.
-------------------------------------------------------------------*/
ModifierArray * EmotionSkeletonModifier::GetModifiers(){

 return &modifiers;
}
