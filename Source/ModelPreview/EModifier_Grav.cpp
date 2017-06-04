/*--------------------------------------------------------------------
  EModifier_Grav.h

  Contains the implementation of the class that modifies the pose of 
  a skeleton based on a gravity direction vector.

  23/04/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#include "stdafx.h"
#include "EModifier_Grav.h"

using namespace Jet;

LOGCLIENT(EModifier_Grav);


/*-------------------------------------------------------------------
PROCEDURE: EModifier_Grav
PURPOSE  : To construct the object using default parameters.
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
EModifier_Grav::EModifier_Grav():
influenceBlend(1.0f)
{

}

/*-------------------------------------------------------------------
PROCEDURE: ~EModifier_Grav
PURPOSE  : To destruct the object. 
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
EModifier_Grav::~EModifier_Grav()
{

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
Modifier::Result EModifier_Grav::Update(float dt){
ParticlePtrArray::Iterator pbi,pbiPrev;
QuatArray::Iterator goi;
Vec3Array::Iterator gpi;
FloatArray::Iterator dai;
Quaternion slerpOrien,origOrien;
Vector3 origPos,tmpVec;
float currInf;

  //Calculate the current overall influence
  currInf=influence*influenceBlend;

  //Init. iterators
  goi=gravOrients.First();
  gpi=gravPositions.First(); 
  dai=distArray.First(); 

  //For each bone, alter by "influence" amount
  pbiPrev=pBones->End();
  for (pbi=pBones->First();pbi!=pBones->End();pbi++){
     
     //Save Bone position and orientationm
     origOrien=pbi->GetOrientation();
     origPos  =pbi->GetPosition();

     
     //Create a slerp between the two orientations
     if(pbiPrev!=pBones->End()) {
     /*
       Quaternion tmpQuat;
       tmpQuat=gravOrientation;
       pbiPrev->GlobalToLocal(tmpQuat); 
       slerpOrien.Slerp(pbiPrev->GetOrientation(),tmpQuat,influence); 
       pbiPrev->SetOrientation(slerpOrien);
     */
     
     
     /*       
       //Save the previous orientation
       Quaternion prevOrien=pbiPrev->GetOrientation();

       //Reset the position so that it has no bearing on orientation calculations
       pbi->SetPosition(*gpi); 

       //Calculate the start position
       Vector3 startPos;
       startPos.Zero(); 
       pbiPrev->SetOrientation(*goi); 
       pbi->LocalToGlobal(startPos);
       

       //Calculate the end position
       Vector3 endPos;
       endPos.Zero(); 
       pbiPrev->SetOrientation(prevOrien); 
       pbi->LocalToGlobal(endPos);

       
       //Calculate the movement
       Vector3 movement;
       movement=endPos-startPos;

       //Only proceed if there is some movement
       if(movement!=movement.GetZero()){ 
         //Calculate how much movement was in the gravity direction
         //Vector3 moveGrav(gravPos);
         //moveGrav=moveGrav.Normalize()*movement.Projection(moveGrav);

         //Calculate the percentage of the movement in the direction of the gravity
         float gravPercent=movement.Projection(gravPos)/movement.Magnitude();  

         //Alter movement in the favour of the gravity direction (by influence amount)
         //   Note: It is possible to do this in a smooth way by cross product
         //movement=movement+((moveGrav-movement) * influence);

         //Adjust the start position by movement amount
         //startPos=startPos+moveGrav;
         /*
         //Calculate the direction vector
         Vector3 dirVect,eularOrien;
         dirVect.Zero(); 
         //pbiPrev->LocalToGlobal(dirVect); 
         dirVect=startPos;//-dirVect;
         pbiPrev->SetOrientation(*goi);
         pbiPrev->GlobalToLocal(dirVect);
         eularOrien=(*goi).GetEulerAngles();
         //eularOrien.x=0.0f;//roll
         eularOrien.y=(float)atan(dirVect.y/sqrtf(dirVect.x*dirVect.x+dirVect.z*dirVect.z));//pitch
         //eularOrien.z=(float)atan(dirVect.z/sqrtf(dirVect.x*dirVect.x+dirVect.y*dirVect.y));//yaw
         prevOrien.Set(eularOrien);
         pbiPrev->SetOrientation(prevOrien); 
         */

         //if(pbiPrev->GetName()==PString("b.bicep.l")){
            //float test=movement.Projection(gravPos);
            //float test2=movement.Magnitude();
            //LOGWARN(("b.bicep.l: %f/%f = %f ",movement.Projection(gravPos),movement.Magnitude(),gravPercent));
         //}
         /*
         if(gravPercent>0.0f){
           slerpOrien.Slerp(*goi,prevOrien,0.9finfluence); 
           pbiPrev->SetOrientation(slerpOrien);
           //*goi=slerpOrien;
         }else{
           slerpOrien.Slerp(*goi,prevOrien,0.1finfluence); 
           pbiPrev->SetOrientation(slerpOrien);
           //*goi=slerpOrien;
         }
        
       }else{
         pbiPrev->SetOrientation(*goi);
       }
       //pbi->SetPosition(origPos); 
       */
     }

     //pbi->SetPosition(((origPos-*gpi)*(1-influence)) + *gpi); 

     if(pbiPrev!=pBones->End()){

         //Recalculate the distance (see if bone is closer)
         tmpVec.Zero();
         pbi->LocalToGlobal(tmpVec);
         (*pBones->First())->GlobalToLocal(tmpVec);
         tmpVec-=gravPos;
         float val=tmpVec.MagnitudeSquared();
         if(val < *dai){
           *dai=val;
           *goi=pbiPrev->GetOrientation(); 
           *gpi=origPos;

         }else{

           slerpOrien.Slerp(pbiPrev->GetOrientation(),*goi,currInf); 
           pbiPrev->SetOrientation(slerpOrien);
           pbi->SetPosition(((origPos-*gpi)*(1-currInf)) + *gpi); 
         }
     }

     //Increment iterators
     goi++;
     gpi++;
     dai++;

     pbiPrev=pbi;
  }



 return OK;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadData
PURPOSE  : To load the data for this modifier.
PRE : em is valid. (ie not NULL)
POST: Returns true if the modifier is loaded successfully, 
      false if not.
-------------------------------------------------------------------*/
bool EModifier_Grav::LoadData(EmotionModifier * em,float emotionInfluence, PString realPath,PString objectLoad){
ParticlePtrArray::Iterator pbi,pbiPrev;
Vector3 tmpVec;
//Matrix3 directMat;

  //Load the gravity position
  ///directMat.Identity();
  //directMat.RotateX(180.0f);
  //gravOrientation.Set(directMat); 

  gravPos.x=0.0f;
  gravPos.y=-10.0f;
  gravPos.z=5.0f;

  //gravPos.x=0.0f;
  //gravPos.y=0.0f;
  //gravPos.z=-10.0f;


  //Set the influence
  influence=emotionInfluence;

  //Save the bone structure
  pBones=em->GetBoneStruct(); 

  //Go to each bone and get initial positions/orientations
  pbiPrev=pBones->End();
  for (pbi=pBones->First();pbi!=pBones->End();pbi++){
     
     //Save the orientation and position
     if(pbiPrev!=pBones->End()){
       gravOrients.Add(Quaternion(pbiPrev->GetOrientation()));
       tmpVec.Zero();
       pbi->LocalToGlobal(tmpVec);
       tmpVec-=gravPos;
       distArray.Add(tmpVec.MagnitudeSquared());
     }else{
       gravOrients.Add(Quaternion());
       distArray.Add(0.0f);
     }
     gravPositions.Add(Vector3(pbi->GetPosition()));

     pbiPrev=pbi;
  }


  //Set the init flag
  init=true;

 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: SetGravPosition
PURPOSE  : To set the gravity position.
PRE : TRUE
POST: The gravity position has been set.
-------------------------------------------------------------------*/
void EModifier_Grav::SetGravPosition(Vector3 pos){
ParticlePtrArray::Iterator pbi,pbiPrev;
unsigned int i;
Vector3 tmpVec;
  
  //Assign the gravity position  
  gravPos=pos;

  //Re-assign the distance calculations (could do this better by 
  //     calculating the difference between the new and old value and adding)
  i=0;
  pbiPrev=pBones->End();
  for (pbi=pBones->First();pbi!=pBones->End();pbi++){
     
     //Save the orientation and position
     if(pbiPrev!=pBones->End()){
       tmpVec.Zero();
       pbi->LocalToGlobal(tmpVec);
       tmpVec-=gravPos;
       distArray[i]=tmpVec.MagnitudeSquared();
     }
     pbiPrev=pbi;
     i++;
  }


 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetInfluenceBlend
PURPOSE  : To set the blend value for when the influence is applied.
PRE : TRUE
POST: The blend value has been set.
-------------------------------------------------------------------*/
void EModifier_Grav::SetInfluenceBlend(float ib){
  influenceBlend=ib;
 return;
}

