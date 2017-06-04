/*--------------------------------------------------------------------
  SkeletonVertex.h

  Contains the implementation of the class that allows the 
  displaying of a each vertex in a ParticleStructure Skeletion

  23/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_SKELETION_VERTEX_H_
#define _INCLUDED_SKELETION_VETTEX_H_

#include <Texture.h>
#include <Display.h>
#include <Viewport.h>
#include <RenderKernel.h>
#include <ResourceDatabase.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>
#include <GeometryBaseInstance.h>
#include <CollisionResult.h>
#include <IndexedMeshInstance.h>

using namespace Jet;

class SkeletonVertex 
{
public:
    
    //Constructor/Destructor
    SkeletonVertex();
    virtual ~SkeletonVertex();

    //New methods
    unsigned int LoadParticleStructure(RenderKernel *rk,ParticleStructure *ps,ResourceDatabase *db,PString show);
    unsigned int GetNumVertices() const;
    void         UnLoadParticleStructure();
    void         RemoveVertexMeshes();
    void         SetScale(float s);

    //Methods to get data on the selected vertex
    bool    IsVertexSelected();
    PString GetVertexName();
    Vector3 GetVertexOrientation();
    bool    SetVertexOrientation(Vector3 newOrien);

    bool TestCollision(Line testLine,Vector3 refPos=Vector3(0.0f,0.0f,0.0f));

private:

    //Internal data gathering method
    void GetBones(Particle *p);
    bool LoadVertexMeshes();

protected:

    typedef FastDynamicArray<Particle *> ParticlePtrArray;
    typedef FastDynamicArray<ParticleStructure *> ParticleStructPtrArray;

    //Array to store pointers to the bone particles
    ParticlePtrArray pArray;

    //The geometry loaders for the show particle structures 
    GeometryBaseInstance * showGeometry; 

    //Pointers to the particle structure for each vertex
    ParticleStructPtrArray pStrArray; 
    int selectedIndex;

    //The current particle structure
    ParticleStructure *pStruct;
    
    //The number of verticies in the skeleton
    unsigned int numVertices;

    //The renderKernel
    RenderKernel *renderKernel;

    //The scale amount for the vertex meshes
    float scale;
};

#endif 
