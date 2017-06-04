/*--------------------------------------------------------------------
  ModelPreviewTask.h

  Contains the definition of the main application task.

  16/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_MODEL_PREVIEW_TASK_H_
#define _INCLUDED_MODEL_PREVIEW_TASK_H_
#include <math.h>

#include <OS.h>
#include <Task.h>
#include <Input.h>
#include <Camera.h>
#include <ViewPort.h>
#include <Animator.h>
#include <SceneGraph.h>
#include <ParticleStructure.h>
#include <ResourceDatabase.h>
#include <Options.h>

#include "RenderData.h"
#include "ControlDialog.h"
#include "ModelObject.h"
#include "SkeletonFile.h"
#include "SkeletonVertex.h"
#include "SkeletonDisplay.h"
#include "ResourceDatabaseManager.h"


using namespace Jet;
using namespace UI;

//Class declarations
class ControlDialog;

class ModelPreviewTask : public Task  
{

public:

    //Constructor/Destructor
    ModelPreviewTask();
	virtual ~ModelPreviewTask();

    //Load new data
    bool LoadMesh     (PString path,PString name);
    bool LoadAnimation(PString path,PString name);
    bool MoveMesh     (Vector2 v  );
    bool SelectVertexMesh(int x,int y);

    //Set methods
    void SetDetail(int detail);
    void SetSkeleton (bool skele);
    void SetWireframe(bool wire);
    void SetLocalAxis(bool la);
    void SetModelVisiable(bool vis);
    void SetAnimationSpeed(int speed);
    void SetAnimationBackwards(bool bck);
    void SetTransparency(int trans);
    void SetBoneVertex(bool bv);
    bool SetVertexOrientation( int vRoll,int vPitch,int vYaw); 

    bool OpenInitialMesh(PString name);
    void SetCameraDistance(float dist);
    void AllignCamera();

    void SaveOrientation();
    void LoadOrientation();

    void CloseTask();
protected:

    //Overridden methods
    virtual bool Init(OS *os);
    virtual void Done();
    virtual UpdateCode Update(float dt);


protected:
    bool CreateWorld(OS *os);
    
    //Standard rendering variables
    RenderKernel      *renderKernel;
    ParticleStructure *world;
    Camera            *camera;
    ViewPort          *viewPort;

    //The manager which loads and manages all the resource databases
    ResourceDatabaseManager DBManager;

    //The resource databases where the model/animation are loaded from
    ResourceDatabase *modelDB;
    ResourceDatabase *animationDB;

    //The model data files
    ModelObject      *model;

    //The dialog where all input comes from
    ControlDialog   *ctrDialog; 

    //The data used in rendering the object and world
    RenderData      *renderData;
    
    //The skeleton representation
    SkeletonDisplay *skeleton;

    //The vertex representation
    SkeletonVertex *skeleVertex;
    ResourceDatabase *skeleVertexDB;

    //The size of the rendering window
    Size windowSize; 

    //Initial load varaibles
    bool initLoad;
    PString initPath,initFilename;


    //If we are done
    bool done;
};


#endif 




