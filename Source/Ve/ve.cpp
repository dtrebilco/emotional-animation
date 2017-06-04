// VE.cpp
//
// This example demonstrates using motion interface.
//

#include "StdAfx.h"
#include <math.h>

// startup support header files
#include <App.h>
#include <OS.h>
#include <OpSysWindows.h>

// render support header files
#include <Location.h>
#include <Display.h>
#include <CollisionResult.h>
#include <GeometryBaseInstance.h>

// user task definition
#include "VE.h"

using namespace Jet;
LOGCLIENT(VE);

const char *resourceDBBaseDir   = "Data";
const char *characterDBName     = "Human";
const char *characterName1      = "kanday_female_heavyfoot.im";
const char *characterName2      = "kanday_male_mediumfoot.im";
const char *mvtConfigName       = "Human.mvt";
const char *cursorDBName        = "Pointer";
const char *terrainDBName       = "Terrain";
const char *objectDBName        = "Objects";
const char *cursorName          = "pointer_fixedres.im";
const char *terrainName         = "terrain_fixedres.im";
const char *treeName            = "firtree.im";
const float boundVSize = 100.0f;

#define MOU_BOUNDARY 5
#define SCROLL_DX    10 *5
#define SCROLL_DY    10 *5
#define ZOOM_STEP    1 *2 


#define NUM_TREES      30
#define NUM_HOUSES     10
#define NUM_CHARACTERS 10

//Global random generateor
Rand randG;

//
// Main Win32 application function
//
int APIENTRY WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
  // Create operating system specific class
  OS windows(instance, cmdLine);

  // Create the application with specified name and operating
  // system specific data.
  App theApp;
  theApp.Init("VE", &windows);

  // create initial user task
  Task *userTask = NewMem(VETask);

  // run main loop
  int result = theApp.Main();

  // release user task
  DeleteMemZ(userTask);

  return result;
}


///////////////////////////////////////////////////////////////////////////////
//
// VECharacter
//
///////////////////////////////////////////////////////////////////////////////

//
// Constructor
//
VECharacter::VECharacter() : 
Modifier(),
skeleton(NULL), 
animator(NULL),
virtET(NULL)
{
}



//
// Destructor
//
VECharacter::~VECharacter()
{
  //DeleteMemZ(VEEmotion);
  DeleteMemZ(animator);
  DeleteMemZ(skeleton);
}



//
// Load character and attach animation and movement interfaces
//
bool VECharacter::Init(VETask *vet,RenderKernel *rk,GeometryBaseInstance & geometry,const ResourceDatabase *database)
{

  //Save a reference to the virtual environment
  virtET=vet;
  renderKernel = rk;

  this->SetHost(renderKernel);

  // Create a copy of the mesh from the loaded data, and attach it to our character.
  if (!geometry.CreateInstance(renderKernel, this))
  {
    LOGERR(("Init: unable to load mesh."))
    return false;
  }
  // Create a skeleton modifier from the loaded mesh so that movement 
  // and animation can blend together
  skeleton = NewMem(EmotionSkeletonModifier(this, renderKernel));

  // Create an animation interface for the movement system to use
  animator = NewMem(Animator(database, skeleton));
  animator->Add(NULL, "run_standard.kin", AnimationModifier::CYCLIC | AnimationModifier::ROOT_HORIZ_OFF, 1.0f, 0.0f, 1.0f);
 
  //Add Emotion
  //VEEmotion=NewMem(EmotionModifier(skeleton));

  //animator->AddObserver(this);

  //Set some initial movement (going no-where) 
  currPos.x=this->GetPosition().x; 
  currPos.y=this->GetPosition().y; 
  MoveTo(currPos.x,currPos.y,0.7f);
  //MoveTo(currPos.x,currPos.y,4.0f);
  return true;
}

//Return the current target
Vector2 VECharacter::GetTarget(){
 return currTarget;

}

//
// Update
//
// Update the particle structure for the time  increment 'dt'
//
Modifier::Result VECharacter::Update(float dt){

   Vector3 newPos;
   Vector2 direction;
   float timeMove = speed*dt;

   //Calculate the direction to go
   direction=currTarget-currPos;


   //Test if we are near the destination
   if(direction.Magnitude() > 0.0f &&
      direction.Magnitude() > timeMove){
     
     //Change the orientation (not working properly??-working badly)
     if(timeMove>0.0f){
       
         if(direction.y ==0.0f){
           direction.y=0.00001f;
         }

         //Check for 90 degrees
         if(fabsf(direction.x/direction.y) <0.2f){
            if(direction.y>0.0f){
                this->SetOrientation(Quaternion(0.0f,0.0f,PI));   
            }else{
                this->SetOrientation(Quaternion(0.0f,0.0f,0.0f));   
            }   
         }else{

             if(direction.x < 0.0f){
               //this->SetOrientation(Quaternion(0.0f,0.0f,(float)cosh(direction.y/direction.Magnitude())+PI/2));
               this->SetOrientation(Quaternion(0.0f,0.0f,(float)tanh(direction.y/direction.x)-PI/2));
             }else{
                 if(direction.x > 0.0f){
                    this->SetOrientation(Quaternion(0.0f,0.0f,(float)tanh(direction.y/direction.x)+PI/2));
                 }
             }
         }
     }
     
     //this->SetOrientation(Quaternion(0.0f,0.0f,1.5*PI));   
     //LOGWARN(("X=%f Y=%f Ang=%f",direction.x,direction.y,(180/PI)*(cosh(direction.x/direction.Magnitude())+PI/2)));


     //If not, move  
     direction.Normalize();
     direction*=timeMove;
     currPos+=direction;

   }else{
     currPos=currTarget;

      //Reset target
      currTarget.x=randG.NextFloat(-75.0f,75.0f);
      currTarget.y=randG.NextFloat(-75.0f,75.0f);
   }

   //Assign the new position
   newPos.x=currPos.x;
   newPos.y=currPos.y;
   newPos.z=virtET->GetHeightAt(currPos.x,currPos.y);
   
   this->SetPosition(newPos);
   
   //Manually hack the bounding volume
   //this->ResetBoundingVolume();
   SetBV(this);

 return OK;
}

//
// Move to the 'destination' location.  Limit speed as required
//
bool VECharacter::MoveTo(float x, float y, float newSpeed){

  currTarget.x=x;
  currTarget.y=y;
  
  //Re-set speed
  if(newSpeed >0.0f){
    //AnimationModifier *am;
    //animator->GetPrimaryAnimation(&am);
    speed=newSpeed;//*am->GetSpeed()*am->GetSpeed()*am->GetSpeed();
  }
  
  return false;
}


//Set the bounding volume to entire world (hack to combat animations messing up)
bool VECharacter::SetBV(Particle *p){
unsigned int i; 
unsigned int numChild;
BoundingVolume *bv;
   
   //if(!p->IsInanimate()){
   //  LOGWARN(("BONE Animate= %s",((const char *)p->GetName())));
     //return true;
   //} 
   bv=NewMem(BoundingVolume(p->GetPosition(), boundVSize));
   bv->SetRadius(boundVSize);
   bv->SetRadiusXY(boundVSize);
   p->SetBoundingVolume(bv);
   p->Show();
   
   //Call on this particles children 
   numChild=p->GetNumChildren();
   for(i=0;i<numChild;i++){
       //Recursive call
       SetBV(p->GetChild(i));
   }


 return false;
}

//Set the movement speed
void VECharacter::SetSpeed(float s){
  speed=s;
}

//Return the EmotionSkeletonModifier
EmotionSkeletonModifier* VECharacter::GetEmotionSkeletonModifier(){

 return skeleton;
}

/*
//
// This method is called when a motion event is triggered.
//
void VECharacter::OnEvent(MotionEventType type)
{
  // Log information about the event
  switch (type)
  {

    case MET_COMPLETE:
    case MET_COMPLETE_BY_PROXY:
    LOGWARN(("Movement complete event"));
      break;


    case MET_ORDER_CHANGE:
      LOGWARN(("Movement change order event"));
      break;
  }
}
*/
/*
//
// This method is called when an animation event is triggered.
//
void VECharacter::OnEvent(AnimationEvent &animEvent){

  //Add a new animation after the old one has completed
  switch (animEvent.GetType()){
    case AET_ANIM_COMPLETE:
        DeleteMemZ(animator);
        // Create an animation interface for the movement system to use
        animator = NewMem(Animator(resDB, skeleton));
        animator->Add(NULL, "run_standard.kin", AnimationModifier::ROOT_HORIZ_OFF, 1.0f, 0.0f, 1.0f);
       break;
  }

 return;
}
//*/

/*-------------------------------------------------------------------
PROCEDURE: Cull
PURPOSE  : To add this object to the visiable list in the view???
PRE : TRUE
POST: If visiable, the object has been added to the view's visiable 
      list.
-------------------------------------------------------------------*/
/*void JETAPI VECharacter::Cull(ViewPort &view, const Matrix4 &trans){

  //Call Base class Cull first to manage the children
  ParticleStructure::Cull(view,trans);

  // Check the visible flag
  //if (visible){

    // Add our object to the visible list in the viewport
    view.AddToVisibleList(VisibleElement(trans, this));

  // Remember that the culled flag is already cleared in our update routine
  //}else{
  //  culled = true;
  //}
 return;
}
*/

/*-------------------------------------------------------------------
PROCEDURE: ResetBoundingVolume
PURPOSE  : To resize the bounding volume to encompass all of the 
           object.
PRE : TRUE
POST: The bounding volume has been resized.
-------------------------------------------------------------------*/
/*void JETAPI VECharacter::ResetBoundingVolume(){

  // Check our children to make sure they are covered by the bounding volume
  ParticleStructure::ResetBoundingVolume();

  // Check if there currently exits a bounding volume
  if (boundingVolume){
      boundingVolume->SetRadius(75.0f);
      boundingVolume->SetRadiusXY(75.0f);     
  }else{
      //Create a new bounding volume
      BoundingVolume *sphere = NewMem(BoundingVolume(GetPosition(), 75.0f));
      sphere->SetRadiusXY(75.0f);
      SetBoundingVolume(sphere);
  }
  
 return;
}
*/
//Changes look of the mesh so that we can tell when it is selected
//This is risky...
void VECharacter::Select(){

    //Change colour of the mesh
    IndexedMeshInstance *mesh =safe_cast<IndexedMeshInstance *>(this->GetChild(0));
    unsigned int numChunks =mesh->GetNumChunks();
    TriangleMeshChunk **chunkArray = mesh->GetChunkArray();

    // Work through the chunks and set the tint (This is just estimation)
    for (uint index = 0; index < numChunks; index++){
      //chunkArray[index]->Tint(renderKernel,Color(0.5f,0.8f,0.0f));
      chunkArray[index]->Tint(renderKernel,Color(0.0f,0.0f,1.0f));
    }

}

//Changes look of the mesh back
//This is risky...
void VECharacter::UnSelect(){

    //Change the colour of the mesh back
    IndexedMeshInstance *mesh =safe_cast<IndexedMeshInstance *>(this->GetChild(0));
    unsigned int numChunks =mesh->GetNumChunks();
    TriangleMeshChunk **chunkArray = mesh->GetChunkArray();

    // Work through the chunks and set the ambient color
    for (uint index = 0; index < numChunks; index++){
      chunkArray[index]->GetMaterial()->ambientCol = Color(1.0f,1.0f,1.0f);

      // Recalculate ambient color of the chunks
      // This will be automatically performed by the LightTarget class demonstrated
      // in the lighting examples
      chunkArray[index]->ResetToAmbient(renderKernel);
    }

}


///////////////////////////////////////////////////////////////////////////////
//
// VETask
//
///////////////////////////////////////////////////////////////////////////////

VETask::VETask() : Task("VETask")
{
  characterDB  = NULL;
  cursorDB     = NULL;
  terrainDB    = NULL;
  terrain      = NULL;
  cursor       = NULL;
  input        = NULL;
  camera       = NULL;
  viewPort     = NULL;
  root         = NULL;
  renderKernel = NULL;

  scrollDX      = 0;
  scrollDY      = 0;

  done         = false;
  emotion=0.0f;
  influence=0.5f;
  selCharacterIndex =-1;
}



//
// Initialize the task. Return false upon critical failure.
//
// This function will create our 3D world and add a character
//
bool VETask::Init(OS *os)
{
  
  //Init random
  randG.Randomise(); 
    
  // First step is to create the 3D world that our mesh will live in
  if (!CreateWorld(os))
  {
    LOGERR(("Init: Unable to create world."));
    return false;
  }

  // Create and initialize the input system
  if (!CreateInputSystem(os))
  {
    LOGERR(("Init: Unable to initialize input system."));
    return false;
  }


  ///////////
  // Terrain

  // Load terrain
  terrainDB = NewMem(ResourceDatabase(terrainDBName, resourceDBBaseDir));
  terrain = Load3dObject(terrainDB, terrainName);
  if (terrain == NULL)
  {
    LOGERR(("Init: Unable to load terrain."));
    return false;
  }

  // Add characters
  if(!AddCharacters()){
    LOGERR(("Init: Unable to load characters."));
    return false;
  }


  // Add objects
  if(!AddObjects()){
    LOGERR(("Init: Unable to load objects."));
    return false;
  }


  //////////
  // Cursor

  // Load a cursor
  cursorDB = NewMem(ResourceDatabase(cursorDBName, resourceDBBaseDir));
  cursor = Load3dObject(cursorDB, cursorName);
  if (cursor == NULL)
  {
    LOGERR(("Init: Unable to load character."));
    return false;
  }

  // Scale cursor
  cursor->SetScale(Vector3(0.5f, 0.5f, 0.5f));

  AdjustCameraPosition();

  //Load initial values into the display
  emotionControl->SetMainEmotion  (emotion);
  emotionControl->SetMainInfluence(influence); 


  // All is well.
  return Task::Init(os);
}

//
// To add all the moving characters
// returns false on failure
//
bool VETask::AddCharacters(){
unsigned int i;
VECharacter *character;

  /////////////
  // Character
  
  // Now load a character so that we have something to move
  characterDB = NewMem(ResourceDatabase(characterDBName, resourceDBBaseDir));

  //Load geometry for both types of characters
  GeometryBaseInstance geometry1,geometry2;
  geometry1.Init(characterDB, characterName1);
  geometry2.Init(characterDB, characterName2);
  for(i=0;i<NUM_CHARACTERS;i++){
    
    //Create a structure for our object and attach it to the world.
    character = NewMem(VECharacter);
    root->Add(character);
    characters.Add(character);

    // Set position of character
    Vector3 target;// = ScrPositionTo3d(Point(windowSize.w/2, windowSize.h/2), 0.0f);
    target.x = randG.NextFloat(-70.0f,70.0f);
    target.y = randG.NextFloat(-70.0f,70.0f);
    target.z = GetHeightAt(target.x, target.y);
    character->SetPosition(target);

    if(randG.NextBool()){ 
        //Load the data (copy mesh)
        if (!character->Init(this,renderKernel,geometry1,characterDB)){
          return false;
        }
    }else{
        //Load the data (copy mesh)
        if (!character->Init(this,renderKernel,geometry2,characterDB)){
          return false;
        }
    }

  }

  //Init the inference engine
  emotionInference=NewMem(VEInferenceEngine);
  emotionInference->Init(&characters,0.0f,360.0f); 
  
  //Get initial values
  emotion=emotionInference->GetGlobalTarget();
  influence=emotionInference->GetGlobalInfluence();

 return true;
}

//
// To add all the trees and houses etc
// returns false on failure
//
bool VETask::AddObjects(){
ParticleStructure *object;
Vector3 objectPos;
unsigned int i;
float scale;

  //Init. the DB.
  objectDB = NewMem(ResourceDatabase(objectDBName, resourceDBBaseDir));
  
  //Load the trees

  // Load the tree mesh
  GeometryBaseInstance geometry;
  geometry.Init(objectDB,treeName);
  for(i=0;i<NUM_TREES;i++){
    // Create a structure for our object and attach it to the world.
    object = NewMem(ParticleStructure);
    root->Add(object);
    objectList.Add(object);

    // Create a copy of the mesh from the loaded data, and attach it to our object.
    if (!geometry.CreateInstance(renderKernel, object)){
      return false;
    }
    scale=randG.NextFloat(0.5f,1.2f); 
    object->SetScale(Vector3(scale,scale,scale));
  }



  //Load the houses
  GeometryBaseInstance geometry2;
  geometry2.Init(objectDB, "kanday_cottage.im");
  for(i=0;i<NUM_HOUSES;i++){
    // Create a structure for our object and attach it to the world.
    object = NewMem(ParticleStructure);
    root->Add(object);
    objectList.Add(object);

    // Create a copy of the mesh from the loaded data, and attach it to our object.
    if (!geometry2.CreateInstance(renderKernel, object)){
      return false;
    }
  }

  
  //Set the heights and positions for all the objects
  for(i=0;i<objectList.Count();i++){
     objectPos.x=randG.NextFloat(-70.0f,70.0f);
     objectPos.y=randG.NextFloat(-70.0f,70.0f);
     objectPos.z=GetHeightAt(objectPos.x,objectPos.y);
     objectList[i]->SetPosition(objectPos);
  }

  return true;
}

//
// Create our rendering framework
// return false on failure
//
bool VETask::CreateWorld(OS *os)
{
  // Create and initialize the render system.
  renderKernel = NewMem(RenderKernel);
  if (!renderKernel->Init(os))
    return false;
  
  //Create the emotion control
  emotionControl=NewMem(ECanvas());
  if(!emotionControl->Init()){
    return false;   
  }


  // Create a virtual scene. This is the base of the tree
  // that we populate with objects (like meshes, lights)
  root = NewMem(ParticleStructure);
  renderKernel->GetSceneGraph()->AddScene(root);

  // Then we have to setup a camera that defines the "eye" from
  // which we wish to look upon the virtual world.
  camera = NewMem(Camera);
  camera->SetPosition(Vector3(0.0f, -70.0f, 10.0f));
  camera->SetOrientation(Quaternion(PI/4, 0.0f, 0.0f));

  // Setup the camera to look at the world that we just created.
  camera->SetScene(root);


  //
  // Before we can render anything we need to set up a viewport
  // that defines the area of the client region where we want
  // something to be drawn. Default view rect is the full client
  // area. This viewport is automatically registered with the
  // layer manager.
  //
  viewPort = NewMem(ViewPort(camera));

  return true;
}


//
// Create and initialize the input system
//
bool VETask::CreateInputSystem(OS *os)
{
  // Create and initialize the input system
  input.Attach(NewMem(InputObserverSystem));
  if (!input->Init(os)) return false;

  // Set the mouse restriction
  Display *display=renderKernel->GetDisplay();
  windowSize = display->GetSize();
  input->SetBounds(Rect(0, 0, windowSize.w-1, windowSize.h-1));

  // Add a observer to the input system.
  input->Add(this, 0);

  mouPosition.x = windowSize.w/2;
  mouPosition.y = windowSize.h/2;

  return true;
}



//
// Load a 3d object
//
ParticleStructure *VETask::Load3dObject(ResourceDatabase *database, const char *name)
{
  ParticleStructure *object;

  // Create a structure for our object and attach it to the world.
  object = NewMem(ParticleStructure);
  root->Add(object);

  // Load a mesh
  GeometryBaseInstance geometry;
  geometry.Init(database, name);

  // Create a copy of the mesh from the loaded data, and attach it to our actor.
  if (!geometry.CreateInstance(renderKernel, object))
  {
    return NULL;
  }

  return object;
}


//
// Clean up the task. Should be able to handle not being initialized.
//
void VETask::Done()
{
  if (input) input->Done();
  DeleteMemZ(emotionInference);
  DeleteMemZ(emotionControl);
  DeleteMemZ(terrain);
  DeleteMemZ(cursor);
  DeleteMemZ(viewPort);
  DeleteMemZ(camera);
  DeleteMemZ(renderKernel);
  DeleteMemZ(characterDB);
  DeleteMemZ(terrainDB);
  DeleteMemZ(cursorDB);
  DeleteMemZ(objectDB);
}



//
// The update method is called by the TaskManager with an amount of
// time for the task to resynchronize to.  Overload this Method to
// perform your processing and return CONTINUE, STOP to stop this
// task from updating or EXIT to quit the from the TaskManager.
//
Task::UpdateCode VETask::Update(float dt){
unsigned int i;

  static float time = 0.0f;

  time += dt;
  if (time > 0.020f){
    time = 0.0f;

    // Execute scroll
    Scroll(scrollDX,scrollDY);
  }

  //Update all character collisions
  for(i=0;i<characters.Count();i++){
     UpdateCollision(characters[i]);
  }

  return done ? Task::EXIT : Task::CONTINUE;
}


//Crude collision detection
void VETask::UpdateCollision(VECharacter *c){
Sphere bv,bv2;
unsigned int i;

  //Update the collisions 
  bv.SetCenter(c->GetPosition());
  bv.SetRadius  (1.0f);
  bv.SetRadiusXY(1.0f);
  bv2.SetRadius  (3.0f);
  bv2.SetRadiusXY(3.0f);
  

  //Loop for all non-moveable objects
  for(i=0;i<objectList.Count();i++){
    bv2.SetCenter(objectList[i]->GetPosition());
    
    //Test for collision  
    if(bv.TestCollision(bv2)){
      //Reset target
      c->MoveTo(randG.NextFloat(-fabsf(c->GetTarget().x),fabsf(c->GetTarget().x)),
                randG.NextFloat(-fabsf(c->GetTarget().y),fabsf(c->GetTarget().y)),
                -1.0f);
            
      break;
    }
  }

 return;
}

/////////////////////
// Camera and cursor

//
// SetCursorPosition
//
bool VETask::SetCursorPosition(const Point &pos)
{
  // Calculate a vector with the normalized screen coordinates
  Vector2 normalized(pos.x / float(windowSize.w), pos.y / float(windowSize.h));

  // Get the line that passes through the
  // normalized screen coordinate (x,y)
  Line line = camera->GetLine(normalized.x, normalized.y);

  // Check if terrain is visible at this point
  CollisionResult collision;
  collision.Initialise();
  if (terrain->TestCollision(line, collision))
  {
    cursor->SetPosition(collision.point);
    mouPosition = pos;
  }
  else
  {
    input->SetMousePos(mouPosition);
    return false;
  }

  cursor->ResetBoundingVolume();

  return true;
}


//
// Scroll the camera in the given direction, but only when the camera remains in
// its movement ranges.
//
bool VETask::Scroll(int dx, int dy)
{
  if (dx || dy)
  {
    // Calculate new position of the camera
    Vector3 pos = camera->GetPosition();
    Vector3 newLookAtPoint = ScrPositionTo3d(Point(windowSize.w/2+dx, windowSize.h/2-dy), pos.z-10.0f);
    Vector3 oldLookAtPoint = ScrPositionTo3d(Point(windowSize.w/2, windowSize.h/2), pos.z-10.0f);
    Vector3 newPos(camera->GetPosition()+(newLookAtPoint-oldLookAtPoint));
    camera->SetPosition(newPos);

    // Check distance of the camera from terrain
    AdjustCameraPosition();

    // Set cursor position after scroll
    if (!SetCursorPosition(mouPosition))
    {
      Point pos(mouPosition.x-dx, mouPosition.y+dy);
      while (!SetCursorPosition(pos))
      {
        pos += Point(-dx, dy);
        if (pos.x < MOU_BOUNDARY) break;
        else if (pos.x > windowSize.w-MOU_BOUNDARY) break;
        if (pos.y < MOU_BOUNDARY) break;
        else if (pos.y > windowSize.h-MOU_BOUNDARY) break;
      }
      input->SetMousePos(pos);
      scrollDX = 0;
      scrollDY = 0;
    }
  }

  return false;
}


//
// Zoom the camera in and out by the specified distance.
//
void VETask::Zoom(float delta)
{
  Vector3 forward = camera->GetForward();
  camera->Translate(delta*forward);
  AdjustCameraPosition();
}


//
// Check distance of the camera from terrain
//
void VETask::AdjustCameraPosition()
{
  Vector3 forward = camera->GetForward();

  // Distance from central point
  Vector3 pos = camera->GetPosition();
  Line line = camera->GetLine(0.5, 0.5);
  CollisionResult collision;
  collision.Initialise();
  if (terrain->TestCollision(line, collision))
  {
    float dist = (pos - collision.point).Magnitude();
    if (dist < 5.0)
    {
      pos += forward*(dist-5.0f);
    }
  }

  // Height over the terrain
  if (fabs(forward.z) > STDMATH_TOLERANCE)
  {
    float height = GetHeightAt(pos.x, pos.y);
    while ((height + 5.0f) - pos.z > 1.0f)
    {
      pos -= forward*ZOOM_STEP;
      height = GetHeightAt(pos.x, pos.y);
    }
  }

  camera->SetPosition(pos);
}


//
// Convert 2d screen position to the 3d
//
Vector3 VETask::ScrPositionTo3d(const Point &pos, float z)
{
  // Calculate a vector with the normalized screen coordinates
  Vector2 normalized(pos.x / float(windowSize.w), pos.y / float(windowSize.h));

  // Get the line that passes through the
  // normalized screen coordinate (x,y)
  Line line = camera->GetLine(normalized.x, normalized.y);

  const Vector3 &direction = line.GetDirection();
  Vector3 point = line.GetPoint();

  if (fabs(direction.z) > STDMATH_TOLERANCE)
  {
    point += direction*((z - point.z)/direction.z);
  }
  else
  {
    point.z = z;
  }

  return point;
}


//
// Get the height of the map at a particular point.
//
float VETask::GetHeightAt(float x, float y)
{
  // Set up a vertical line so that we may call
  // TestCollision to determine the height
  Line line(Vector3(0.0f, 0.0f, -1.0f), Vector3(x, y, 1000.0f));

  // Test the collision of this line with the Particle
  CollisionResult collisionResult;
  collisionResult.Initialise();
  if (terrain->TestCollision(line, collisionResult))
  {
    return collisionResult.point.z;
  }

  return 0.0f;
}


/////////////////
// InputObserver

//
// Called when a key is pressed/unpressed.
//
int VETask::OnKey(KeyEvent key)
{
  if(key.IsReleased()){
    return Propagation::NotHandled;
  }

  switch (key.Code())
  {
    case KC_Esc:   done = true;     break;

    case KC_Left:  emotion+=5.0f;
                   if(emotion>=360.0f){emotion-=360.0f;}
                   if(selCharacterIndex>=0){
                      emotionInference->ModifyEmotionCharacter(selCharacterIndex,emotion,influence);
                      emotionControl->SetEmotion(emotion);
                   }else{
                      emotionInference->ModifyAll(emotion,influence,0.5f); 
                      emotionControl->SetMainEmotion(emotion);
                   }
                   break;

    case KC_Right: emotion-=5.0f; 
                   if(emotion<0.0f){emotion+=360.0f;}
                   emotionControl->SetEmotion(emotion);
                   if(selCharacterIndex>=0){
                      emotionInference->ModifyEmotionCharacter(selCharacterIndex,emotion,influence);
                   }else{
                      emotionInference->ModifyAll(emotion,influence,0.5f); 
                      emotionControl->SetMainEmotion(emotion);
                   }
                   break;

    case KC_Up:    influence+=0.05f; 
                   if(influence>1.0f){influence=1.0f;}
                   
                   if(selCharacterIndex>=0){
                      emotionInference->ModifyEmotionCharacter(selCharacterIndex,emotion,influence);
                      emotionControl->SetInfluence(influence);
                   }else{
                      emotionInference->ModifyAll(emotion,influence,0.5f);
                      emotionControl->SetMainInfluence(influence);
                   }
                   break;

    case KC_Down:  influence-=0.05f; 
                   if(influence<0.0f){influence=0.0f;}
                   
                   if(selCharacterIndex>=0){
                      emotionInference->ModifyEmotionCharacter(selCharacterIndex,emotion,influence);
                      emotionControl->SetInfluence(influence);
                   }else{
                      emotionInference->ModifyAll(emotion,influence,0.5f); 
                      emotionControl->SetMainInfluence(influence);
                   }
                   break;

    case KC_M   :  emotionControl->SetVisible(!emotionControl->IsVisible());
                   break;
    case KC_KeypadPlus:   Zoom( ZOOM_STEP); break;
    case KC_KeypadMinus:  Zoom(-ZOOM_STEP); break;
  }
  return Propagation::Handled;
}


//
// Called when the mouse position changes and/or mouse button is pressed/unpressed.
//
int VETask::OnMouse(MouseEvent mouse, const Point &pos)
{
  if (mouse.Code() == ME_Moved)
  {
    // Set position of the cursor
    SetCursorPosition(pos);
  }


  ///////////
  // Scroll

  // Check cursor position
  scrollDX = 0;
  scrollDY = 0;
  if (mouPosition.x < MOU_BOUNDARY) scrollDX = -SCROLL_DX;
  else if (mouPosition.x > windowSize.w-MOU_BOUNDARY) scrollDX = SCROLL_DX;
  if (mouPosition.y < MOU_BOUNDARY) scrollDY = SCROLL_DY;
  else if (mouPosition.y > windowSize.h-MOU_BOUNDARY) scrollDY = -SCROLL_DY;


  ////////////
  // Selection of character

  if (mouse.Code() == ME_LeftClick){
    Sphere bv,bv2;
    unsigned int i;

      //Update the collisions 
      bv.SetCenter(cursor->GetPosition());
      bv.SetRadius  (1.0f);
      bv.SetRadiusXY(1.0f);
      bv2.SetRadius  (1.0f);
      bv2.SetRadiusXY(1.0f);

      //Unselect existing character
      if(selCharacterIndex>=0){
         characters[selCharacterIndex]->UnSelect();
         selCharacterIndex =-1;
         emotion  =emotionInference->GetGlobalTarget(); 
         influence=emotionInference->GetGlobalInfluence(); 
          
         //Reset display for this character
         emotionControl->SetMainEmotion(emotion);
         emotionControl->SetMainInfluence(influence);
         emotionControl->Setworldchar(false);  
      }

      //Loop for all non-moveable objects
      for(i=0;i<characters.Count();i++){
        bv2.SetCenter(characters[i]->GetPosition());
    
        //Test for collision  
        if(bv.TestCollision(bv2)){
          //Indicate selection
          selCharacterIndex=(int)i;
          characters[i]->Select();
          emotion  =emotionInference->GetEmotionTarget(i); 
          influence=emotionInference->GetEmotionInfTarget(i); 
          
          //Reset display for this character
          emotionControl->SetEmotion(emotion);
          emotionControl->SetInfluence(influence);
          emotionControl->Setworldchar(true);  
          break;
        }
      }
  }

  return Propagation::Handled;
}
