/*--------------------------------------------------------------------
  ControlDialog.cpp

  Contains the definition of the class that controls the major dialog.

  16/03/2001 -- Created the file

TODO: Line 83 Delete control database

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "StdAfx.h"
#include "ControlDialog.h"

using namespace Jet;
using namespace UI;

const char *CONTROL_DIALOGNAME = "Main";
const char *CONTROL_RESOURCEDATABASEPATH = "";
const char *CONTROL_RESOURCENAME = "Data";
const char *FILE_DIALOG_START_DIR = "c:\\";
//const char *FILE_DIALOG_START_DIR = "c:\\c_prog\\jetbeta2\\src\\Examples\\SampleData\\Human\\";

LOGCLIENT(ControlDialog);

/*-------------------------------------------------------------------
PROCEDURE: ControlDialog
PURPOSE  : To construct the object
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
ControlDialog::ControlDialog():
modelDisplay(NULL),
renderData(NULL),
mainForm(NULL),
interfaceSystem(NULL),
controlDatabase(NULL),
isHidden(false),
currFileDialog(NULL),
currDir(FILE_DIALOG_START_DIR),
message(NULL),
LMBPressed(false),
RMBPressed(false),
MMBPressed(false)
{

}
/*-------------------------------------------------------------------
PROCEDURE: ~ControlDialog
PURPOSE  : To destroy the object
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
ControlDialog::~ControlDialog(){

  FreeResources();

}

/*-------------------------------------------------------------------
PROCEDURE: FreeResources
PURPOSE  : To free data associated with this object
PRE : TRUE
POST: All possible data that could be de-allocated, is de-allocated.
-------------------------------------------------------------------*/
void ControlDialog::FreeResources(){

  //Release the Main form
  if (mainForm){ 
      mainForm->DoClose();
      mainForm=NULL; 
  }

  // Cleanup interface system, this will ensure that outstanding
  //   dialogs are removed and therefore no resources in our database
  //   are being accessed.
  if (interfaceSystem){
    interfaceSystem->Done();
    interfaceSystem->ReleaseReference();
    interfaceSystem=NULL;
  }

  //Delete data base
  if(controlDatabase){
     DeleteMemZ(controlDatabase);
  }

}

/*-------------------------------------------------------------------
PROCEDURE: Init
PURPOSE  : To initialise the dialog.
PRE : "m" and "r" are valid objects.
POST: If returns true, the dialog has been init.
-------------------------------------------------------------------*/
bool ControlDialog::Init(RenderKernel *rk,ModelPreviewTask * m,RenderData * rd){
 
  //Firts try and free resources from an unsuccessful init.
  FreeResources();

  //Assign initial variables
  modelDisplay=m;
  renderData=rd;

  //Request the interface system from the plugin system via it's unique id.
  Plugin<InterfaceSystemFactory> interfaceSystemFactory(InterfaceSystem::uid);
  if (!interfaceSystemFactory){ 
      return false;
  }

  //Create and initialize the interface system
  interfaceSystem = interfaceSystemFactory->CreateInstance();
  
  //Initialize the interface system
  if(!interfaceSystem->Init(rk->GetOS(),rk)){
      LOGERR(("ControlDialog::Init() Unable to init interface system"));
      FreeResources();
      return false;
  }
  
 
  //Create a resource database from which to get resources
  controlDatabase = NewMem(ResourceDatabase(CONTROL_RESOURCENAME,CONTROL_RESOURCEDATABASEPATH));
    
  //Open an instance of the dialog from the specified database
  mainForm = interfaceSystem->Open(CONTROL_DIALOGNAME,controlDatabase);

  //mainForm =safe_cast<Dialog *>(interfaceSystem->CreateInstance(CONTROL_DIALOGNAME));

  //Stop if the dialog cannot be found
  if (!mainForm){
    FreeResources();
    return false;
  }
 
  //Add this as an interface for key and mouse
  interfaceSystem->GetInputObserverSystem()->Add(this,0);
  
  //Get the size of the main dialog and set initial position
  mainForm->GetControl("DMain")->GetSize(width,height);
  mainForm->GetControl("DMain")->SetPos(0,0);

  //Show the model tab first
  safe_cast<GControl *>(mainForm->GetControl("TModel"))->Select();
  
  //Bind the escape key for this dialog to our quit routine
  mainForm->BindKey(KC_Esc,CALLCPP(ControlDialog,Exit));

  //Bind test load and save keys
  mainForm->BindKey(KC_L,CALLCPP(ControlDialog,LoadOrientation));
  mainForm->BindKey(KC_S,CALLCPP(ControlDialog,SaveOrientation));

  //Bind all the other controls
  mainForm->GetControl("BExit")->SetCallback("OnClick",CALLCPP(ControlDialog,Exit));
  mainForm->GetControl("BHide")->SetCallback("OnClick",CALLCPP(ControlDialog,Hide));
  
  //Model controls 
  mainForm->GetControl("BLoadModel")->SetCallback("OnClick" ,CALLCPP(ControlDialog,OpenMesh));
  mainForm->GetControl("CRotate"   )->SetCallback("OnSelect",CALLCPP(ControlDialog,OnModelRotate));
  mainForm->GetControl("CkOrbit"   )->SetCallback("OnClick" ,CALLCPP(ControlDialog,OnOrbitRotate));
  mainForm->GetControl("SLPolyPercent")->SetCallback("OnUpdate" ,CALLCPP(ControlDialog,OnDetailMove));
  mainForm->GetControl("SLTransparency")->SetCallback("OnUpdate" ,CALLCPP(ControlDialog,OnTransparency));
    
  //Animation controls
  mainForm->GetControl("BLoadAnimation")->SetCallback("OnClick",CALLCPP(ControlDialog,OpenAnimation));
  mainForm->GetControl("SLSpeed")->SetCallback("OnUpdate" ,CALLCPP(ControlDialog,OnSpeedMove));
  mainForm->GetControl("CkReverse"   )->SetCallback("OnClick" ,CALLCPP(ControlDialog,OnBackwards));

  //Camera controls
  mainForm->GetControl("BZoom+")->SetCallback("OnClick" ,CALLCPP(ControlDialog,OnCameraInc));
  mainForm->GetControl("BZoom-")->SetCallback("OnClick" ,CALLCPP(ControlDialog,OnCameraDec));

  //Display controls
  mainForm->GetControl("CkSkeleton" )->SetCallback("OnClick" ,CALLCPP(ControlDialog,OnSkeleton));
  mainForm->GetControl("CkWireFrame")->SetCallback("OnClick" ,CALLCPP(ControlDialog,OnWireframe));
  mainForm->GetControl("CkLocalAxis")->SetCallback("OnClick" ,CALLCPP(ControlDialog,OnLocalAxis));
  mainForm->GetControl("CkModel"    )->SetCallback("OnClick" ,CALLCPP(ControlDialog,OnModelDisplayed));

  //Bone vertex controls
  mainForm->GetControl("CkEnableBoneVertex")->SetCallback("OnClick" ,CALLCPP(ControlDialog,OnBoneVertex));
  mainForm->GetControl("SLRoll" )->SetCallback("OnUpdate" ,CALLCPP(ControlDialog,OnRollMove));
  mainForm->GetControl("SLPitch")->SetCallback("OnUpdate" ,CALLCPP(ControlDialog,OnPitchMove));
  mainForm->GetControl("SLYaw"  )->SetCallback("OnUpdate" ,CALLCPP(ControlDialog,OnYawMove));

 return true;
}

/*-------------------------------------------------------------------
PROCEDURE: OnKey
PURPOSE  : To process key events
PRE : Init. has been called successfully.
POST: If we handled the key returns Propagation::Handled 
      else returns Propagation::NotHandled
-------------------------------------------------------------------*/
int ControlDialog::OnKey(KeyEvent key){

 return Propagation::NotHandled;
}

/*-------------------------------------------------------------------
PROCEDURE: OnMouse 
PURPOSE  : To process mouse events.
PRE : Init. has been called successfully.
POST: If we handled the mouse event, returns Propagation::Handled 
      else returns Propagation::NotHandled
-------------------------------------------------------------------*/
int ControlDialog::OnMouse(MouseEvent mouse, const Point &pos){
int handled = Propagation::NotHandled;

  //Find what the event was
  switch(mouse.Code()){
 
    //If left button prres, save position and indicate button down
    case(ME_LeftPress)  : LMBPressed=true;
                          mouPrevPos=pos;
                          mouLDown=pos;
                          handled=Propagation::Handled;
                          break;

    //If left button release, indicate button up and test if the position has changed
    //  If so, try to prform a selection
    case(ME_LeftRelease): LMBPressed=false;
                          handled=Propagation::Handled;
                          if(mouLDown.Distance(pos) < 2.0f){
                            modelDisplay->SelectVertexMesh(pos.x,pos.y);
                            UpdateBoneVertexTab();
                          }
                          break;

    //If right button prres, save position and indicate button down
    case(ME_RightPress)  : RMBPressed=true;
                          mouPrevPos=pos; 
                          handled=Propagation::Handled;
                          break;

    //If right button release, indicate button up
    case(ME_RightRelease): RMBPressed=false;
                          handled=Propagation::Handled;
                          break;

    //If the mouse moves while the left button is down, rotate the mesh
    // but if it moves while right is down, zoom
    case(ME_Moved)      : if(LMBPressed){
                             handled=Propagation::Handled;
                             Point mov = pos-mouPrevPos;
                             modelDisplay->MoveMesh(Vector2((float)mov.x,
                                                            (float)mov.y));   
                             mouPrevPos=pos;
                             break;
                          }
                          if(RMBPressed){
                             handled=Propagation::Handled;
                             Point mov = pos-mouPrevPos;
                             modelDisplay->
                                 SetCameraDistance(renderData->cameraDist-((float)mov.y/100.0f));   
                             mouPrevPos=pos;
                             UpdateCameraTab();
                             break; 
                          }
                          //modelDisplay->SelectVertexMesh(pos.x,pos.y);
                          break;

  }

 return handled;
}

/*-------------------------------------------------------------------
PROCEDURE: Exit 
PURPOSE  : To terminate the application.
PRE : Init has been successfully called.
POST: The application will now close.
-------------------------------------------------------------------*/
void ControlDialog::Exit(){
  //Just close the main task
  modelDisplay->CloseTask(); 
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: Hide 
PURPOSE  : To hide/unhide the dialog interface.
PRE : Init has been successfully called.
POST: The dialog is hidden/unhidden
-------------------------------------------------------------------*/
void ControlDialog::Hide(){

  //Test if the dialog is hidden or not
  if(!isHidden){
    
    //Resize and change button lettering
    mainForm->GetControl("DMain")->SetSize(width,44);
    safe_cast<Button *>(mainForm->GetControl("BHide"))->SetValueString("UnHide");
    mainForm->SetOpacity(0.5f);  
    isHidden=true;

  }else{
    
    //Resize and change button lettering
    mainForm->GetControl("DMain")->SetSize(width,height);
    safe_cast<Button *>(mainForm->GetControl("BHide"))->SetValueString("Hide");
    isHidden=false;
    mainForm->SetOpacity(1.0f);  
  }
  
 return;
}


/*-------------------------------------------------------------------
PROCEDURE: FileDialogClose 
PURPOSE  : To handle the file dialog closing.
PRE : Init has been successfully called and a valid file dialog is 
      currently open.
POST: All actions needed to be performed have been.
-------------------------------------------------------------------*/
void ControlDialog::FileDialogClose(){

  //Make the main control solid
  mainForm->SetOpacity(1.0f); 

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OpenMesh 
PURPOSE  : To open and load a new mesh file.
PRE : Init has been successfully called.
POST: If a valid file was selected, it is loaded.
-------------------------------------------------------------------*/
void ControlDialog::OpenMesh(){
   
  //Create the file dialog (this is a guess...)
  currFileDialog = safe_cast<DialogFiles *>(interfaceSystem->Open("Files",controlDatabase));

  //If we could not create teh dialog
  if (!currFileDialog){
    LOGERR(("ControlDialog::OpenMesh() Unable to find dialog Files"));
    return;
  }


  //Make the main control transparent
  mainForm->SetOpacity(0.5f); 

  //Set dialog callback and variables 
  currFileDialog->SetCallback("OnOpen",CALLCPP(ControlDialog,LoadMesh));
  currFileDialog->SetCallback("OnClose",CALLCPP(ControlDialog,FileDialogClose));
  currFileDialog->SetDirectory(currDir);
  currFileDialog->SetWildcards("*.?m");
  currFileDialog->SetFileName(".");
  currFileDialog->RefreshFiles();
  //currFileDialog->SetResourceDatabase(controlDatabase);

  //Init the dialog
  if(!currFileDialog->Init()){
      LOGERR(("ControlDialog::OpenMesh() Unable to init dialog files"));
      return; 
  }
 
  return;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadMesh
PURPOSE  : To load a meshusing the current file dialog.
PRE : Init has been successfully called, "currFileDialog" contains a 
      valid object.
POST: The file in "currFileDialog" has been attempted to be loaded.
-------------------------------------------------------------------*/
void ControlDialog::LoadMesh(){

  //Try to load the mesh
  if(modelDisplay->LoadMesh(currFileDialog->GetDirectory(),currFileDialog->GetFileName())){ 

    //Update the visual display
    UpdateModelTab();
    UpdateAnimationTab();
    UpdateCameraTab();
    UpdateBoneVertexTab();

    //Also save the directory
    currDir=currFileDialog->GetDirectory(); 

    //Close the dialog  
    currFileDialog->DoClose();
    currFileDialog=NULL;

  }else{

    //Close the dialog  
    currFileDialog->DoClose();
    currFileDialog=NULL;
      
    MessageLoad("Unable to open file/Invalid file");
  }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OpenAnimation
PURPOSE  : To open and load a new animation file.
PRE : Init has been successfully called.
POST: If a valid file was selected, it is loaded.
-------------------------------------------------------------------*/
void ControlDialog::OpenAnimation(){

  //Create the file dialog (this is a guess...)
  currFileDialog = safe_cast<DialogFiles *>(interfaceSystem->Open("Files",controlDatabase));
  
  //If we could not create the dialog
  if (!currFileDialog){
    LOGERR(("ControlDialog::OpenMesh() Unable to find dialog Files"));
    return;
  }

  //Make the main control transparent
  mainForm->SetOpacity(0.5f); 

  //Set dialog callback and variables 
  currFileDialog->SetCallback("OnOpen",CALLCPP(ControlDialog,LoadAnimation));
  currFileDialog->SetCallback("OnClose",CALLCPP(ControlDialog,FileDialogClose));
  currFileDialog->SetDirectory(currDir);
  currFileDialog->SetWildcards("*.kin");
  currFileDialog->SetFileName(".");
  currFileDialog->RefreshFiles();
  //currFileDialog->SetResourceDatabase(controlDatabase);

  //Init the dialog
  if(!currFileDialog->Init()){
      LOGERR(("ControlDialog::OpenMesh() Unable to init dialog files"));
      return; 
  }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadAnimation
PURPOSE  : To load a animation using the current file dialog.
PRE : Init has been successfully called, "currFileDialog" contains a 
      valid object.
POST: The file in "currFileDialog" has been attempted to be loaded.
-------------------------------------------------------------------*/
void ControlDialog::LoadAnimation(){

  //Try to load the mesh
  if(modelDisplay->LoadAnimation(currFileDialog->GetDirectory(),currFileDialog->GetFileName())){ 

    //Update the displayed data   
    UpdateAnimationTab();

    //Also save the directory
    currDir=currFileDialog->GetDirectory(); 

    
    //Close the dialog  
    currFileDialog->DoClose();
    currFileDialog=NULL;

  }else{
    //Close the dialog  
    currFileDialog->DoClose();
    currFileDialog=NULL;
      
    MessageLoad("Unable to open file/Invalid file/No loaded mesh");
  }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: MessageClose
PURPOSE  : To close the message dialog
PRE : TRUE
POST: The message dialog is closed
-------------------------------------------------------------------*/
void ControlDialog::MessageClose(){
    //Close the message
    if(message){
      //Make the main control solid
      mainForm->SetOpacity(1.0f); 

      //Close the dialog
      message->SetEvents(false);
      message->DoClose();
      message=NULL;
    }
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: MessageLoad
PURPOSE  : To display a message to the user
PRE : TRUE
POST: The message is displayed if possible.
-------------------------------------------------------------------*/
void ControlDialog::MessageLoad(PString comment){

  //Ensure there is not two messages at once
  MessageClose();

  //Report that we could not load the file
  message=interfaceSystem->Open("SMessage",controlDatabase);

  //If we could not open the message
  if(!message){
      return;
  }

  //Make the main control transparant
  mainForm->SetOpacity(0.5f); 

  //Bind all controls
  message->GetControl("BOK")->SetCallback("OnClick",CALLCPP(ControlDialog,MessageClose));
   
  //safe_cast<Jet::Window *>(message->GetControl("DMessage"))->SetCaption("Error");
  safe_cast<Static *>(message->GetControl("SText"))->SetValueString(comment);

  //Lock onto this dialog
  message->SetEvents(true);

 return;
}


/*-------------------------------------------------------------------
PROCEDURE: OnOrbitRotate
PURPOSE  : To handle the checking or un-checking of the orbit button.
PRE : Init. has been called successfully.
POST: The orbit flag has been adjusted.
-------------------------------------------------------------------*/
void ControlDialog::OnOrbitRotate(){

  //Load the orbit selection
  Value * orbitValue = safe_cast<Value *>(mainForm->GetControl("CkOrbit"));
  renderData->orbit=orbitValue->GetValueBool();

 return;
}


/*-------------------------------------------------------------------
PROCEDURE: OnModelRotate
PURPOSE  : To handle the model rotation radio button selection.
PRE : TRUE
POST: The selection has been handled.
-------------------------------------------------------------------*/
void ControlDialog::OnModelRotate(){

  Value * value = safe_cast<Value *>(mainForm->GetControl("CRotate"));

  //Assign the value to the render data
  switch(value->GetValueInt()){
     case 0: renderData->movement=ROTATE_ALL; 
             break;
     case 1: renderData->movement=ROTATE_X; 
             break;
     case 2: renderData->movement=ROTATE_Y; 
             break;
     case 3: renderData->movement=ROTATE_Z; 
             break;
     
  }

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnDetailMove
PURPOSE  : To handle the moving of the detail slider.
PRE : TRUE
POST: The movement has been handled.
-------------------------------------------------------------------*/
void ControlDialog::OnDetailMove(){

  //Get the current value from the slider
  renderData->currDetailPercent=safe_cast<Value *>(mainForm->GetControl("SLPolyPercent"))->GetValueInt();
  
  //If we are close to 100%
  if(renderData->currDetailPercent>=95){
    renderData->currDetailPercent=100;
  }

  //Pass the detail to the model
  modelDisplay->SetDetail(renderData->currDetailPercent); 

  //Update the model data
  UpdateModelTab();

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: UpdateModelTab
PURPOSE  : To update the Model Tab on the main form.
PRE : Init. has been called successfully.
POST: The current data from "renderData" has been updated.
-------------------------------------------------------------------*/
void ControlDialog::UpdateModelTab(){

  //If successful, put the file name in the user interface
  safe_cast<Static *>(mainForm->GetControl("SModel"))->SetValueString(renderData->currModelName);

  //Also put the number of polygons (not really model tab but related)
  char buf[32];
  PString pNum(itoa(renderData->currPoly,buf,10));
  PString pPercent(itoa(renderData->currDetailPercent,buf,10));

  safe_cast<Static *>(mainForm->GetControl("SNumPoly"))->
                     SetValueString(pNum + " (" + pPercent + "%)");

  //Reset the slider
  mainForm->GetControl("SLPolyPercent")->SetDisabled(!renderData->scaleDetail);
  mainForm->GetControl("SLPolyPercent")->RefreshConfig();
  safe_cast<Slider *>(mainForm->GetControl("SLPolyPercent"))->SetValueInt(renderData->currDetailPercent);

  //Also set transparency
  PString pTrans(itoa(renderData->transparency,buf,10));

  safe_cast<Static *>(mainForm->GetControl("STransparency"))->
                     SetValueString(pTrans + "%");

  //Reset the slider
  safe_cast<Slider *>(mainForm->GetControl("SLTransparency"))->SetValueInt(renderData->transparency);

  //Load the rotation selection
  Value * rotateValue = safe_cast<Value *>(mainForm->GetControl("CRotate"));
  switch(renderData->movement){
    case(ROTATE_ALL) : rotateValue->SetValueInt(0);
                       break;

    case(ROTATE_X  ) : rotateValue->SetValueInt(1);
                       break;

    case(ROTATE_Y  ) : rotateValue->SetValueInt(2);
                       break;
    
    case(ROTATE_Z  ) : rotateValue->SetValueInt(3);
                       break;
  }

  //Load the orbit selection
  Value * orbitValue = safe_cast<Value *>(mainForm->GetControl("CkOrbit"));
  orbitValue->SetValueBool(renderData->orbit);

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: UpdateAnimationTab
PURPOSE  : To update the Animation Tab on the main form.
PRE : Init. has been called successfully.
POST: The current data from "renderData" has been updated.
-------------------------------------------------------------------*/
void ControlDialog::UpdateAnimationTab(){

  //Put the file name in the user interface
  safe_cast<Static *>(mainForm->GetControl("SAnimation"))->SetValueString(renderData->currAnimationName);

  //Update speed slider
  char buf[32];
  PString pPercent(itoa(renderData->speed,buf,10));

  safe_cast<Static *>(mainForm->GetControl("SSpeed"))->
                     SetValueString(pPercent + "%");

  
  safe_cast<Slider *>(mainForm->GetControl("SLSpeed"))->SetValueInt(renderData->speed);

  //Update reverse switch
  Value * reverseValue = safe_cast<Value *>(mainForm->GetControl("CkReverse"));
  reverseValue->SetValueBool(renderData->reverse);

  //Update load onto vertex check box

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: UpdateCameraTab
PURPOSE  : To update the Camera Tab on the main form.
PRE : Init. has been called successfully.
POST: The current data from "renderData" has been updated.
-------------------------------------------------------------------*/
void ControlDialog::UpdateCameraTab(){

  //Update distance reporting slider
  char buf[32];
  PString pDist(gcvt(renderData->cameraDist, 6,buf));

  safe_cast<Static *>(mainForm->GetControl("SCamDistance"))->
                     SetValueString(pDist);


 return;
}

/*-------------------------------------------------------------------
PROCEDURE: UpdateBoneVertexTab
PURPOSE  : To update the Bone Vertex Tab on the main form.
PRE : Init. has been called successfully.
POST: The current data from "renderData" has been updated.
-------------------------------------------------------------------*/
void ControlDialog::UpdateBoneVertexTab(){

  //Update the enable check box
  Value * boneVValue = safe_cast<Value *>(mainForm->GetControl("CkEnableBoneVertex"));
  boneVValue->SetValueBool(renderData->showVertex);

  //Update name
  safe_cast<Static *>(mainForm->GetControl("SVertexName"))->
                     SetValueString(renderData->vertexName);



  //Upadate angle sliders
  //Update Roll
  char buf[32];
  PString pValue(itoa(renderData->vertexRoll,buf,10));

  safe_cast<Static *>(mainForm->GetControl("SVRoll"))->
                     SetValueString(pValue);
  
  safe_cast<Slider *>(mainForm->GetControl("SLRoll"))->SetValueInt(renderData->vertexRoll+180);


  //Update Pitch
  pValue = (itoa(renderData->vertexPitch,buf,10));

  safe_cast<Static *>(mainForm->GetControl("SVPitch"))->
                     SetValueString(pValue);
  
  safe_cast<Slider *>(mainForm->GetControl("SLPitch"))->SetValueInt(renderData->vertexPitch+180);


  //Update Yaw
  pValue = (itoa(renderData->vertexYaw,buf,10));

  safe_cast<Static *>(mainForm->GetControl("SVYaw"))->
                     SetValueString(pValue);
  
  safe_cast<Slider *>(mainForm->GetControl("SLYaw"))->SetValueInt(renderData->vertexYaw+180);

 return;
}
/*-------------------------------------------------------------------
PROCEDURE: OnSpeedMove
PURPOSE  : To handle the moving of the speed slider.
PRE : Init. has been called successfully.
POST: The movement has been handled.
-------------------------------------------------------------------*/
void ControlDialog::OnSpeedMove(){

  //Get the current value from the slider
  renderData->speed=safe_cast<Value *>(mainForm->GetControl("SLSpeed"))->GetValueInt();
  
  //If we are close to 200%
  if(renderData->speed>=195){
    renderData->speed=200;
  }

  //Pass the detail to the model
  modelDisplay->SetAnimationSpeed(renderData->speed); 

  //Update the model data
  UpdateAnimationTab();

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnBackwards
PURPOSE  : To handle the clicking of the backwards button.
PRE : Init. has been called successfully.
POST: The the click has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnBackwards(){

  //Load the selection
  Value * bckValue = safe_cast<Value *>(mainForm->GetControl("CkReverse"));
  renderData->reverse=bckValue->GetValueBool();
  modelDisplay->SetAnimationBackwards(renderData->reverse);
  
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnSkeleton
PURPOSE  : To handle the clicking of the skeleton check button.
PRE : Init. has been called successfully.
POST: The the click has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnSkeleton(){
  
  //Load the selection
  Value * skeleValue = safe_cast<Value *>(mainForm->GetControl("CkSkeleton"));
  renderData->skeleton=skeleValue->GetValueBool();
  modelDisplay->SetSkeleton(renderData->skeleton);
  
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnSkeleton
PURPOSE  : To handle the clicking of the wireframe check button.
PRE : Init. has been called successfully.
POST: The the click has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnWireframe(){

  //Load the selection
  Value * wireValue = safe_cast<Value *>(mainForm->GetControl("CkWireFrame"));
  renderData->wireframe=wireValue->GetValueBool();
  modelDisplay->SetWireframe(renderData->wireframe);
  
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnLocalAxis
PURPOSE  : To handle the clicking of the local axis check button.
PRE : Init. has been called successfully.
POST: The the click has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnLocalAxis(){

  //Load the selection
  Value * axisValue = safe_cast<Value *>(mainForm->GetControl("CkLocalAxis"));
  renderData->localaxis=axisValue->GetValueBool();
  modelDisplay->SetLocalAxis(renderData->localaxis);

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnModelDisplayed
PURPOSE  : To handle the clicking of the model display check button.
PRE : Init. has been called successfully.
POST: The the click has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnModelDisplayed(){

  //Load the selection
  Value * modelValue = safe_cast<Value *>(mainForm->GetControl("CkModel"));
  renderData->modelDisplayed=modelValue->GetValueBool();
  modelDisplay->SetModelVisiable(renderData->modelDisplayed);

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnTransparency
PURPOSE  : To handle the updating of the slider that represents the 
           transparancy of the model.
PRE : Init. has been called successfully.
POST: The the update has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnTransparency(){

  //Get the current value from the slider
  renderData->transparency=safe_cast<Value *>(mainForm->GetControl("SLTransparency"))->GetValueInt();
  
  //If we are close to 100%
  if(renderData->transparency>=95){
    renderData->transparency=100;
  }

  //Pass the value to the model
  modelDisplay->SetTransparency(renderData->transparency); 

  //Update the model data
  UpdateModelTab();

}

/*-------------------------------------------------------------------
PROCEDURE: OnCameraInc
PURPOSE  : To handle the clicking of the button to move the camera 
           away.
PRE : Init. has been called successfully.
POST: The the click has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnCameraInc(){

  //Increase camera distance
  renderData->cameraDist+=2.0f; 

  //Set the camera distance
  modelDisplay->SetCameraDistance(renderData->cameraDist); 

  //Update the tab
  UpdateCameraTab();

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnCameraDec
PURPOSE  : To handle the clicking of the button to move the camera 
           closer.
PRE : Init. has been called successfully.
POST: The the click has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnCameraDec(){

  //Decrease camera distance
  renderData->cameraDist-=2.0f; 

  //Set the camera distance
  modelDisplay->SetCameraDistance(renderData->cameraDist); 

  //Update the tab
  UpdateCameraTab();

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnBoneVertex
PURPOSE  : To handle the clicking of the bone vertyex checkbox.
PRE : Init. has been called successfully.
POST: The the click has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnBoneVertex(){

  //Load the selection
  Value * bvertexValue = safe_cast<Value *>(mainForm->GetControl("CkEnableBoneVertex"));
  renderData->showVertex=bvertexValue->GetValueBool();
  modelDisplay->SetBoneVertex(renderData->showVertex);

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnRollMove
PURPOSE  : To handle the moving of the roll slider.
PRE : Init. has been called successfully.
POST: The the movement has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnRollMove(){

  //Get the current value from the slider
  renderData->vertexRoll=safe_cast<Value *>(mainForm->GetControl("SLRoll"))->GetValueInt();

  //Subtract 180 (so in -180 to 180 degree range)
  renderData->vertexRoll-= 180;

  //Pass the detail to the model
  modelDisplay->SetVertexOrientation(renderData->vertexRoll,
                                     renderData->vertexPitch,
                                     renderData->vertexYaw); 

  //Let's be lazy and update the whole tab
  UpdateBoneVertexTab();


 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnPitchMove
PURPOSE  : To handle the moving of the pitch slider.
PRE : Init. has been called successfully.
POST: The the movement has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnPitchMove(){

  //Get the current value from the slider
  renderData->vertexPitch=safe_cast<Value *>(mainForm->GetControl("SLPitch"))->GetValueInt();

  //Subtract 180 (so in -180 to 180 degree range)
  renderData->vertexPitch-= 180;

  //Pass the detail to the model
  modelDisplay->SetVertexOrientation(renderData->vertexRoll,
                                     renderData->vertexPitch,
                                     renderData->vertexYaw); 

  //Let's be lazy and update the whole tab
  UpdateBoneVertexTab();


 return;
}

/*-------------------------------------------------------------------
PROCEDURE: OnYawMove
PURPOSE  : To handle the moving of the yaw slider.
PRE : Init. has been called successfully.
POST: The the movement has been handled
-------------------------------------------------------------------*/
void ControlDialog::OnYawMove(){

  //Get the current value from the slider
  renderData->vertexYaw=safe_cast<Value *>(mainForm->GetControl("SLYaw"))->GetValueInt();


  //Subtract 180 (so in -180 to 180 degree range)
  renderData->vertexYaw-= 180;

  //Pass the detail to the model
  modelDisplay->SetVertexOrientation(renderData->vertexRoll,
                                     renderData->vertexPitch,
                                     renderData->vertexYaw); 

  //Let's be lazy and update the whole tab
  UpdateBoneVertexTab();

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SetCurrDirectory
PURPOSE  : To set the current directory for loading files from.
PRE : Init. has been called successfully.
POST: The directory path has been changed.
-------------------------------------------------------------------*/
void ControlDialog::SetCurrDirectory(PString path){
  currDir=path;
 return;
}

/*-------------------------------------------------------------------
PROCEDURE: SaveOrientation
PURPOSE  : To save the orientation (TEST METHOD)
PRE : Init has been called successfully.
POST: The orientation has been saved.
-------------------------------------------------------------------*/
void ControlDialog::SaveOrientation(){

  modelDisplay->SaveOrientation(); 

 return;
}

/*-------------------------------------------------------------------
PROCEDURE: LoadOrientation
PURPOSE  : To load the orientation (TEST METHOD)
PRE : Init has been called successfully.
POST: The orientation has been loaded.
-------------------------------------------------------------------*/
void ControlDialog::LoadOrientation(){

  modelDisplay->LoadOrientation(); 
 return;
}
