//
// EmotionObject.h
//
// Contains the definition for an example object containing a scalable object.
//

#ifndef _EMOTION_OBJECT_H
#define _EMOTION_OBJECT_H

#include <ConfigRender3DCore.h>
// Jet headers
#include <ResourceDatabase.h>
#include <RenderKernel.h>
#include <ParticleStructure.h>
#include <Animator.h>

#include "EmotionSkeletonModifier.h"


using namespace Jet;

class EmotionObject : public ParticleStructure
{
public:

  //
  // Constructor
  //
  EmotionObject();
  virtual ~EmotionObject();

  //
  // Load object and initialize it
  //
  bool Init(RenderKernel *renderKernel, ResourceDatabase *database, const PString &name);

  bool LoadAnimation(ResourceDatabase *database,const PString &name);
  
  bool LoadEmotion(unsigned int num);
  void SetInfluence(float newInf,float oldInf);

protected:

  RenderKernel         *renderKernel;     // Access to the render kernel
  EmotionSkeletonModifier *skeleton;      // skeleton to blend animation and movement
  Animator                *animator;      // animation interface
  EmotionModifier         *testEmotion;
  EmotionModifier         *oldtestEmotion;

  unsigned int  currEmotionNum;

};

#endif // !__EmotionObject_H
