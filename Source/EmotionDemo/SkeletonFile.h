/*--------------------------------------------------------------------
  SkeletonFile.h

  Contains the implementation of the class that allows the 
  saving and loading of the skeletal data in a particle structure.

  31/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_SKELETION_FILE_H_
#define _INCLUDED_SKELETION_FILE_H_

#include <File.h>
#include <Vector3.h>   
#include <Quaternion.h>
#include <FastDynamicArray.h>
#include <ParticleStructure.h>

using namespace Jet;

//The type of data written to the file
enum SKELE_DATA_SAVED{

    SKELE_ORIENTATION = 0,
    SKELE_POSITION    = 1,
    SKELE_BOTH        = 2,
};

class SkeletonFile 
{
public:
    
    //Constructor/Destructor
    SkeletonFile();
    virtual ~SkeletonFile();

    //New methods
    static unsigned int LoadSkeleton(ParticleStructure *ps,PString filename);
    static unsigned int SaveSkeleton(ParticleStructure *ps,PString filename,SKELE_DATA_SAVED s);

private:
    typedef FastDynamicArray<Particle *> ParticlePtrArray;

    //Internal data gathering method
    static void GetBones(Particle *p,ParticlePtrArray &pArray);


   
};

#endif 
