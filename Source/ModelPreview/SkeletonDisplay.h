/*--------------------------------------------------------------------
  SkeletonDisplay.h

  Contains the implementation of the class that allows the 
  displaying of a ParticleStructure Skeletion

  09/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_SKELETION_DISPLAY_H_
#define _INCLUDED_SKELETION_DISPLAY_H_

#include <Texture.h>
#include <Display.h>
#include <Viewport.h>
#include <RenderKernel.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>

using namespace Jet;

class SkeletonDisplay : public ParticleStructure  
{
public:
    
    //Constructor/Destructor
    SkeletonDisplay();
	virtual ~SkeletonDisplay();

    //New methods
    unsigned int LoadParticleStructure(ParticleStructure *ps);
    unsigned int GetNumBones() const;
    void         UnLoadParticleStructure();
    void         SetSkeletonColour(Color &c,Display *d=NULL);
    void         SetAutoUpdate(bool update); 
    void         UpdateSkeleton();
    void         SetDisplay(Display *d);

    //Overriden methods
    virtual void JETAPI ResetBoundingVolume();
    virtual void JETAPI Cull(ViewPort &view, const Matrix4 &trans);
    virtual void JETAPI Render(Display *display, ViewPort &viewport, Matrix4 &transformation);


private:

    //Internal data gathering method
    void GetBones(Particle *p,Particle *lastBone=NULL);


protected:

    typedef FastDynamicArray<Particle *> ParticlePtrArray;

    //Array to store pointers to the bone particles
    ParticlePtrArray pArray;

    //The current particle structure
    ParticleStructure *pStruct;

    //The render vertex array
    PackedVertex * pvArray;

    //The display to reneder to
    Display  *display;

    //Misc. data
    bool    autoUpdate;
    Color   colour;
    udword  displayColour;
    
    //This is not the REAL number of bones but 
    //  the number of verticies that make up each bone
    unsigned int numBones;
};

#endif 
