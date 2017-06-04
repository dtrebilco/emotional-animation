/*--------------------------------------------------------------------
  EmotionModifier.cpp

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on specific emotion data.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.

NOTE: Extra speed may be gained by unrolling loops that access all 
      modifiers. However a good compiler should do this automatically.
--------------------------------------------------------------------*/

#include "stdafx.h"
#include "EmotionModifier.h"

using namespace Jet;

LOGCLIENT(EmotionModifier);

/*-------------------------------------------------------------------
PROCEDURE: EmotionModifier
PURPOSE  : To construct the object using default parameters.
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
EmotionModifier::EmotionModifier(EmotionSkeletonModifier *host){
int i;

  //Set the host
  SetHost(host);

  //Init all modifiers to NULL
  for(i=0;i<MOD_MAX;i++){
    eModifiers[i]=NULL;
  }

}

/*-------------------------------------------------------------------
PROCEDURE: ~EmotionModifier
PURPOSE  : To destruct the object. 
           Removes this emotion modifier from the host.
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
EmotionModifier::~EmotionModifier(){
int i;

  //Remove host
  host->RemoveEmotionModifier(this); 

  //Remove any remaining modifiers
  for(i=0;i<MOD_MAX;i++){
    if(eModifiers[i]!=NULL){
      DeleteMem(eModifiers[i]);
    }
  }
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
Modifier::Result EmotionModifier::Update(float dt){
int i;

  //Call update on all modifiers
  for(i=MOD_MIN;i<MOD_MAX;i++){  
    if(eModifiers[i]){
      eModifiers[i]->Update(dt); 
    }
  }

 return OK;
}

/*-------------------------------------------------------------------
PROCEDURE: SetHost
PURPOSE  : To set the host on which this modifier calls upon. 
PRE : TRUE
POST: The host has been set and this object has been registered with
      the host. Also the modifiers have been re-init. for the new 
      host.
-------------------------------------------------------------------*/
Modifier::Result EmotionModifier::SetHost(EmotionSkeletonModifier *h){

  //Assign the host
  host=h;

  //Register with the host
  host->AddEmotionModifier(this);

  //Calculate bone data
  pArray.RemoveAll();
  GetBones(host->GetParticleStruct()); 

 return OK;
}

/*-------------------------------------------------------------------
PROCEDURE: ReInitModifiers
PURPOSE  : To re-init. the modifiers. This does not re-load 
           data from input files but causes the particle-structure 
           data to be re-loaded from the host. This is useful if the 
           target particle structuer has changed. 
           (ie parts have been added/deleted)
PRE : TRUE
POST: The specified modifiers have been re-init.
-------------------------------------------------------------------*/
/*
void EmotionModifier::ReInitModifiers(){

  //Re-Calculate bone data
  pArray.RemoveAll();
  GetBones(host->GetParticleStruct()); 


  //TODO: Loop and re-init

 return;
}
*/

/*-------------------------------------------------------------------
PROCEDURE: LoadEmotionDB
PURPOSE  : To load all modifiers from the specified emotion name with
           the given influence. Object specific data can be specified 
           in a sub directory of "objectLoad".
PRE : eMod is valid.
POST: Returns the number of modifiers loaded.
-------------------------------------------------------------------*/
int EmotionModifier::LoadEmotionDB(float emotionInfluence,PString path,PString emotionName,
                                    PString objectLoad){

 return LoadModifier(MOD_ALL,emotionInfluence,path,emotionName,objectLoad);
}

/*-------------------------------------------------------------------
PROCEDURE: LoadModifier
PURPOSE  : To load specified modifiers from the specified emotion 
           name with the given influence. Object specific data can be
           specified in a sub directory of "objectLoad".
PRE : eMod is valid.
POST: Returns the number of modifiers loaded.
      If the modifier has previously been loaded, it is removed 
      and re-loaded.
-------------------------------------------------------------------*/
int EmotionModifier::LoadModifier(E_MODIFIER_FLAG eMod,float emotionInfluence,PString path,PString emotionName,
                                   PString objectLoad){
int i,count=0;
PString realPath;

  assert(eMod<=MOD_MAX && eMod>=MOD_MIN);

  realPath=path+ "/" + emotionName;

  //To load all current modifiers
  if(eMod==MOD_ALL){
 
    //Loop and load all modifiers
    for(i=MOD_MIN;i<MOD_MAX;i++){  
      count+=LoadModifier((E_MODIFIER_FLAG)i,emotionInfluence,path,emotionName,objectLoad);
    }

   return count;

  //Load specific modifier
  }else{
    //Test if the modifier exists
    if(eModifiers[eMod]!=NULL){
       DeleteMemZ(eModifiers[eMod]);
    }
       
    //Load the modifier 
    switch(eMod){
       case(MOD_POSE): 
           eModifiers[eMod] = NewMem(EModifier_Pose()); 
           break;

       case(MOD_AF):   
           eModifiers[eMod] = NewMem(EModifier_AF()); 
           break;

       case(MOD_GRAV):   
           eModifiers[eMod] = NewMem(EModifier_Grav()); 
           break;

       case(MOD_RAND):   
           eModifiers[eMod] = NewMem(EModifier_Rand()); 
           break;
    }

    //If the modifier loads successfully
    if(eModifiers[eMod]!=NULL){ 

       //Try to load the modifier, else delete it  
       if(eModifiers[eMod]->LoadData(this,emotionInfluence,realPath,objectLoad)){
          return 1;
       }else{
          DeleteMemZ(eModifiers[eMod]);
       }
    }
  }

 return 0;
}


/*-------------------------------------------------------------------
PROCEDURE: UnLoadModifier
PURPOSE  : To unload specified modifiers.
PRE : eMod is valid.
POST: Returns the number of modifiers unloaded.
-------------------------------------------------------------------*/
int EmotionModifier::UnLoadModifier(E_MODIFIER_FLAG eMod){
int i,count=0;

  assert(eMod<=MOD_MAX && eMod>=MOD_MIN);

  //To unload all current modifiers
  if(eMod==MOD_ALL){

    //Loop and unload all modifiers
    for(i=MOD_MIN;i<MOD_MAX;i++){  
      if(eModifiers[i]!=NULL){
        DeleteMemZ(eModifiers[eMod]);
        count++;
      }
    }
  }else{

    //Unload single modifier
    if(eModifiers[eMod]!=NULL){
       DeleteMemZ(eModifiers[eMod]);
       count++;
    }
  }

 return count;

}

/*-------------------------------------------------------------------
PROCEDURE: SetModifierInfluence
PURPOSE  : To set a particular modifier's influence factor.
PRE : eMod is valid.
POST: Returns the number of modifiers that have their influence 
      changed.
-------------------------------------------------------------------*/
int EmotionModifier::SetModifierInfluence(E_MODIFIER_FLAG eMod,float emotionInfluence){
int i,count=0;

  assert(eMod<=MOD_MAX && eMod>=MOD_MIN);

  //To set all current modifiers
  if(eMod==MOD_ALL){

    //Loop and set all modifiers
    for(i=MOD_MIN;i<MOD_MAX;i++){  
      if(eModifiers[i]!=NULL){
         eModifiers[i]->SetInfluence(emotionInfluence);
         count++;
      }
    }
  }else{

    //Set specific modifier
    if(eModifiers[eMod]!=NULL){
       eModifiers[eMod]->SetInfluence(emotionInfluence);
       count++;
    }
  }

 return count;
}

/*-------------------------------------------------------------------
PROCEDURE: GetParticleStruct
PURPOSE  : To get the particle structure that this skeleton uses.
PRE : TRUE
POST: The current particle structure is returned.
-------------------------------------------------------------------*/
ParticleStructure * EmotionModifier::GetParticleStruct(){

 return host->GetParticleStruct(); 
}

/*-------------------------------------------------------------------
PROCEDURE: GetBoneStruct
PURPOSE  : To get the underlying bone structure of the particle 
           structure.
PRE : TRUE
POST: An array of all the bones for the current particle structure 
      is returned.
-------------------------------------------------------------------*/
ParticlePtrArray  * EmotionModifier::GetBoneStruct(){

 return &pArray;
}

/*-------------------------------------------------------------------
PROCEDURE: GetHost
PURPOSE  : To host of this emotion modifier.
PRE : TRUE
POST: The host is returned.
-------------------------------------------------------------------*/
EmotionSkeletonModifier * EmotionModifier::GetHost(){

 return host;
}

/*-------------------------------------------------------------------
PROCEDURE: GetBones
PURPOSE  : To get the bones in a particle structure. Assumes bone's 
           name start with "b.". This method currently uses recursion 
           so it may be slow. The algromithm is depth first traversial.
PRE : The particle passed is valid.
POST: The particle array "pArray" has been populated with all 
      particles that are bones.
-------------------------------------------------------------------*/
void EmotionModifier::GetBones(Particle *p) {
unsigned int i; 
unsigned int numChild;
    
   //Get the first 2 characters of this bone's name
   PString pname=p->GetName();
   pname.Left(2);
     
   //If this particle is a bone,  
   if(pname.Compare("b.")==0){

     //Add the bone to the array
     pArray.Add(p);
   }
   
    
   //Call on this particles children 
   numChild=p->GetNumChildren();
   for(i=0;i<numChild;i++){
       //Recursive call
       GetBones(p->GetChild(i));
   }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: GetModifier
PURPOSE  : To get a requested modifier.
PRE : eMod is valid.
POST: The requested modifier is returned. If the modifier does not 
      exist, NULL is returnedThe flag MOD_ALL is ignored.

NOTE: This method to is to modify specific modifier flags and it is 
      not intended that references be saved on the returned modifier 
      as it may be deleted or changed. (ie hacky)
-------------------------------------------------------------------*/
EModifier * EmotionModifier::GetModifier(E_MODIFIER_FLAG eMod){

  //Just return NULL for the all flag
  if(eMod==MOD_ALL){
    return NULL;
  }

  //Return the modifier
  return eModifiers[eMod];
}