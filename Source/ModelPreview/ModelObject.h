/*--------------------------------------------------------------------
  ModelObject.h

  Contains the definition of the class that loads and maintains an 
  object within a scene.

  17/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#ifndef __MODEL_OBJECT_H
#define __MODEL_OBJECT_H

#include "stdafx.h"

#include <RenderKernel.h>
#include <ResourceDatabase.h>
#include <ParticleStructure.h>
#include <ScalableDetail.h>
#include <SkeletonModifier.h>
#include <Animator.h>

#include <ConfigRender3DCore.h>
#include <IndexedMeshInstance.h>
#include <GeometryBaseInstance.h>

#include "EmotionModifier.h"
#include "EmotionSkeletonModifier.h"

using namespace Jet;

class ModelObject : public ParticleStructure
{

public:
 
    //Constructor/Destructor
    ModelObject(RenderKernel *rk);
    virtual ~ModelObject(); 

    //New methods
    bool LoadMesh     (ResourceDatabase *database,const PString &name);
    bool LoadAnimation(ResourceDatabase *database,const PString &name,const PString boneName="");

    int GetNumTriangles() const;
    int GetMaxTriangles() const;

    void SetDetail(float detail);
    void SetScale(float scale);
    bool IsDetailScaleable();

    void SetAnimationSpeed(float speed);
    void SetAnimationBackwords(bool bck);

    Vector2 GetXLimits();
    Vector2 GetYLimits();
    Vector2 GetZLimits();

    void CentreModel();

protected:

    void FindLimits();

  //The render kernel
  RenderKernel         *renderKernel;         // Access to the render kernel
  
  //Pointers to the same model (Enables quick access to different features)
  IndexedMeshInstance  *mesh;  
  ScalableDetail       *object;

  //The skeleton modifier and animation interface
  EmotionSkeletonModifier *skeleton;
  Animator                *animator;
  EmotionModifier         *testEmotion;

  Vector2 xLimits,yLimits,zLimits;
};

#endif 
