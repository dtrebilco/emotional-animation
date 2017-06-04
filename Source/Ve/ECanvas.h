/*--------------------------------------------------------------------
  ECanvas.h

  Contains the emotion canvas.

  08/06/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_E_CANVAS_H_
#define _INCLUDED_E_CANVAS_H_

#include <Overlay.h>
//#include <Canvas.h>
#include <File.h>
#include <PString.h>
#include <Vector3.h>   
#include <Modifier.h>
#include <Quaternion.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>
#include <AnimationModifier.h>

#include <RenderKernel.h>
#include <DisplayTexture.h>
#include <Bitmap.h>
#include <Display.h>

using namespace Jet;

class ECanvas : public Overlay
{
public:
	ECanvas();
	virtual ~ECanvas();

    bool Init();
    virtual void Render(); 
    void SetEmotion(float emo);
    void SetInfluence(float inf);
    void SetMainEmotion(float emo);
    void SetMainInfluence(float inf);
    void Setworldchar(bool wc);

private:
    DisplayTexture *texture1,*texture2,*texture3;
    PackedVertex vertexArray[4];

    float emotion,influence;
    float mainEmotion,mainInfluence;
    bool  selectChar;

};

#endif 
