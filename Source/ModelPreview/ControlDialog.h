/*--------------------------------------------------------------------
  ControlDialog.h

  Contains the definition of the class that controls the major dialog.

  16/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_CONTROL_DIALOG_H_
#define _INCLUDED_CONTROL_DIALOG_H_
#include "stdafx.h"
#include <stdlib.h>

#include <Input.h>
#include <RenderKernel.h>
#include <ResourceDatabase.h>

#include <File.h>
#include <CallCpp.h>
#include <2DTypes.h>
#include <InputObserver.h>
#include <InputObserverSystem.h>
#include <UI/InterfaceSystem.h>
#include <UI/Dialog.h>
#include <UI/Button.h>
#include <UI/Window.h>
#include <UI/DialogFiles.h>
#include <UI/Slider.h>
#include <UI/GControl.h>

#include "RenderData.h"
#include "ModelPreviewTask.h"

//Class declarations
class ModelPreviewTask;

using namespace Jet;
using namespace UI;

class ControlDialog : public InputObserver 
{
public:
    //Constructor/Destructor
    ControlDialog();
	virtual ~ControlDialog();

    //New methods
    bool Init(RenderKernel *rk,ModelPreviewTask * m,RenderData * rd);


    // InputObserver virtual methods
    virtual int OnKey(KeyEvent key);
    virtual int OnMouse(MouseEvent mouse, const Point &pos);

    void SetCurrDirectory(PString path);

    //The view update methods
    void UpdateModelTab();
    void UpdateAnimationTab();
    void UpdateCameraTab();
    void UpdateBoneVertexTab();

protected:
    void FreeResources();

    //The control methods
    void Hide();
    void Exit();
    void OpenMesh();
    void LoadMesh();
    void OpenAnimation();
    void LoadAnimation();

    void OnModelRotate();
    void OnOrbitRotate();
    void OnDetailMove();
    void OnSpeedMove();
    void OnBackwards();

    void OnSkeleton();
    void OnWireframe();
    void OnLocalAxis();
    void OnModelDisplayed();
    void OnTransparency();
    void OnBoneVertex();

    void OnCameraInc();
    void OnCameraDec();

    void OnRollMove();
    void OnPitchMove();
    void OnYawMove();

    void LoadOrientation();
    void SaveOrientation();

    //Message 
    void MessageClose();
    void MessageLoad(PString comment);
    void FileDialogClose();

    ModelPreviewTask *modelDisplay; 
    RenderData       *renderData;

    // Input system
    //InputObserverSystemRef  input;   
    Dialog           *mainForm;
    ResourceDatabase *controlDatabase; 
    InterfaceSystem  *interfaceSystem;

    //Hidden command variables 
    bool isHidden;
    int width,height;

    //The current dialog for files
    DialogFiles *currFileDialog;
    PString      currDir;
    Dialog      *message;

private:

    //Mouse data
    Point   mouPrevPos,mouLDown; 
    bool LMBPressed,RMBPressed,MMBPressed;


};

#endif 
