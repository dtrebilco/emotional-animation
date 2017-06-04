//
// EmotionDemo.cpp
//
// This example demonstrates the progressive mesh technology in Jet.
//


#include "StdAfx.h"

#include "EmotionDemo.h"
// Startup support header files
#include <App.h>
#include <Display.h>
#include <OS.h>
#include <OpSysWindows.h>
#include <UI/Font.h>



//
// All Jet classes are encapsulated in the 'Jet' namespace.  Before any Jet objects
// are used, the following namespace must be imported.
//
using namespace Jet;



//
// Define the logging client name for this example
//
LOGCLIENT(Emotion);

const char SAMPLE_DATADIR[]   = "Data"; // Location of parent sample data directory
const char OBJECT_DBNAME[]    = "Human";            // Names of resource sub-directories
const char OBJECT_NAME[]      = "kanday_female_heavyfoot.pm"; // Names of resource to load
const char INTERFACE_DBNAME[] = "Interface";
const char FONT_NAME[] = "font8a.tga.font";         // Name of font resource


///////////////////////////////////////////////////////////////////////////////
// Task implementation

//
// EmotionDemoTask
//
// Construct the task
//
EmotionDemoTask::EmotionDemoTask() : Task("EmotionDemoTask"),
  done(false),
  lastConsoleUpdate(0.0f),
  console(NULL),
  objectDB(NULL),
  interfaceDB(NULL),
  object(NULL),
  camera(NULL),
  viewPort(NULL),
  root(NULL),
  renderKernel(NULL),
  emotionNum(0),
  currEmoInf(1.0f),
  currEmotionName("None"),
  currAnimName("Idle")
{
}

//
// Do the grunt work in setting up a functional virtual 3d world
// using JET.
// Return false on failure.
//
bool EmotionDemoTask::CreateWorld(OS *os)
{
  //
  // First step is to initialize the JET systems we need.
  // In this case we only need the render system.
  //
  renderKernel = NewMem(RenderKernel);
  if (!renderKernel->Init(os))
    return false;

  //
  // Create a virtual scene. This is the base of the tree
  // that we populate with objects (like meshes, lights,
  // 3d sounds etc).
  //
  root = NewMem(ParticleStructure);
  renderKernel->GetSceneGraph()->AddScene(root);

  Display *display = renderKernel->GetDisplay();
  display->Set2DView(-1.0f, 1.0f, -1.0f, 1.0f);

  //
  // Then we have to setup a camera that defines the "eye" from
  // which we wish to look upon the virtual world.
  //
  camera = NewMem(Camera);
  camera->SetPosition(Vector3(1.0f, -3.0f, 2.0f));
  camera->AlignToTarget(Vector3(0.0f, 0.0f, 1.0f));

  // Setup the camera to look at the world (mesh) that we just created.
  camera->SetScene(root);

  //
  // Before we can render anything we need to set up a viewport
  // that defines the area of the client region where we want
  // something to be drawn. Default view rect is the full client
  // area. This viewport is automatically registered with the
  // layer manager.
  //
  viewPort = NewMem(ViewPort(camera));

//  viewPort->SetSize(10,10);
  return true;
}



//
// Initialize the task. Return false upon critical failure.
//
bool EmotionDemoTask::Init(OS *os)
{
  // Setup the world.
  if (!CreateWorld(os))
  {
    LOGERR(("EmotionDemoTask::Init() Unable to create world"));
    return false;
  }

  // Initialize the resource databases
  InitDatabases();

  // Load the interface components.
  if (!InitInterface(os))
  {
    LOGERR(("EmotionDemoTask::Init() Unable to create interface"));
    return false;
  }


  //
  // Now use the object class created in the example and add it to the world.  
  //
  object = NewMem(EmotionObject);

  if (!object->Init(renderKernel, objectDB, OBJECT_NAME))
  {
    LOGERR(("EmotionObject::Init() Unable to load mesh"));
    DeleteMemZ(object);
    return false;
  }
  root->Add(object);

  // All is well.
  return Task::Init(os);
}



//
// Initialize all the resource databases
//
void EmotionDemoTask::InitDatabases()
{
  // Initialize the object database
  objectDB = NewMem(ResourceDatabase(OBJECT_DBNAME, SAMPLE_DATADIR));

  // Initialize the interface database
  interfaceDB = NewMem(ResourceDatabase(INTERFACE_DBNAME, SAMPLE_DATADIR));
}



//
// Create the user interface objects. Return false on failure
//
bool EmotionDemoTask::InitInterface(OS *os)
{
  //////////////////////
  // Setup the console

  // Create a font
  FontInstance font;
  font.Init(interfaceDB, FONT_NAME);

  // Create and initialize the console
  console = NewMem(OSDConsole("Output Console", 1));
  console->Init(font);               // initialize console's font
  console->Init(Size(80, 4));        // initialize console's dimensions
  console->SetPos(Point(5, 5));      // set screen position
  console->SetOpacity(0.7f);         // set overall opacity


  ///////////////////////////
  // Setup the input system

  // Create and initialize the input system
  input.Attach(NewMem(InputObserverSystem));
  if (!input->Init(os)) 
  {
    LOGERR(("EmotionDemoTask::InitInterface() Unable to initialize input system"));
    return false;
  }

  // Add an observer to the input system.
  input->Add(this, 0);

  return true;
}


//
// Clean up the task. Should be able to handle not being initialized.
//
void EmotionDemoTask::Done()
{

  DeleteMemZ(console);
  //DeleteMemZ(object);
  DeleteMemZ(interfaceDB);
  DeleteMemZ(objectDB);
  DeleteMemZ(camera);
  DeleteMemZ(viewPort);
  DeleteMemZ(renderKernel);
}



//
// Watch for user input to end program.
//
Task::UpdateCode EmotionDemoTask::Update(float dt)
{


  // Update the status console periodically with the current number of triangles
  // in the object
  lastConsoleUpdate += dt;
  if (lastConsoleUpdate > 0.02f){

    console->Clear();
    //console->Goto(0,0);
    //console->Printf("Console print %f",dt);

    lastConsoleUpdate = 0.0f;

    console->Printf("Current Animation: %s ",(const char*)currAnimName);
    console->Printf("\nCurrent Emotion: %s ",(const char*)currEmotionName);
    //console->Printf("\nCurrent Emotion: Emotion%d ",emotionNum);
    
    if(currEmoInf<1.0f){
      console->Printf("\nCurrent Emotion: %d%% ",(int)(currEmoInf*100.0f));
      currEmoInf+=0.02f; 
      if(currEmoInf>1.0f){
          currEmoInf=1.0f;
      }
      
    }
    
    if(oldEmoInf>0.0f){
      console->Printf("\nOld Emotion: %d%% ",(int)(oldEmoInf*100.0f)); 
      oldEmoInf-=0.02f;
      if(oldEmoInf<0.0f){
          oldEmoInf=0.0f;
      }
      object->SetInfluence(currEmoInf,oldEmoInf); 
    }
    

  }

  

//console->Printf("Num of Childern %d ",object->GetChild(0)->GetNumChildren());

  return (done)?Task::EXIT:Task::CONTINUE;
}



//
// Called when a key is pressed/unpressed.
// Return 'true' to prevent other observers from receiving this event.
//
int EmotionDemoTask::OnKey(KeyEvent key)
{
  
  if(!key.IsReleased()){
     return Propagation::NotHandled;
  }

  // Set control and 
  switch (key.Code())
  {
    case KC_Esc: 
      // Exit example
      done = true;     
      break;
    
    case KC_1 : object->LoadEmotion(0); 
                emotionNum=0;currEmoInf=0.0f;oldEmoInf=1.0f;
                object->SetInfluence(currEmoInf,oldEmoInf);
                currEmotionName=PString("None"); 
                break;
    case KC_2 : object->LoadEmotion(1); 
                emotionNum=1;currEmoInf=0.0f;oldEmoInf=1.0f;
                object->SetInfluence(currEmoInf,oldEmoInf); 
                currEmotionName=PString("Angry"); 
                break;
    case KC_3 : object->LoadEmotion(2); 
                emotionNum=2;currEmoInf=0.0f;oldEmoInf=1.0f;
                object->SetInfluence(currEmoInf,oldEmoInf); 
                currEmotionName=PString("Calm"); 
                break;
    case KC_4 : object->LoadEmotion(3); 
                emotionNum=3;currEmoInf=0.0f;oldEmoInf=1.0f;
                object->SetInfluence(currEmoInf,oldEmoInf); 
                currEmotionName=PString("Depressed"); 
                break;
    case KC_5 : object->LoadEmotion(4); 
                emotionNum=4;currEmoInf=0.0f;oldEmoInf=1.0f;
                object->SetInfluence(currEmoInf,oldEmoInf); 
                currEmotionName=PString("Happy"); 
                break;
    case KC_6 : object->LoadEmotion(5); 
                emotionNum=5;currEmoInf=0.0f;oldEmoInf=1.0f;
                currEmotionName=PString("Face wind"); 
                break;
    case KC_7 : object->LoadEmotion(6); 
                emotionNum=6;currEmoInf=0.0f;oldEmoInf=1.0f;   
                currEmotionName=PString("Skitzoid/Cold"); 
                break;
    case KC_8 : object->LoadEmotion(7); 
                emotionNum=7;currEmoInf=0.0f;oldEmoInf=1.0f;   
                currEmotionName=PString("Drunk"); 
                break;
    case KC_9 : object->LoadEmotion(8); 
                emotionNum=8;currEmoInf=0.0f;oldEmoInf=1.0f;   
                currEmotionName=PString("Scrurry"); 
                break;
    case KC_Q : object->LoadAnimation(objectDB,"idle_normal.kin");
                currEmoInf=1.0f;oldEmoInf=0.0f;
                currAnimName=PString("Idle");
                break;
    case KC_W : object->LoadAnimation(objectDB,"walk_standard.kin");
                currEmoInf=1.0f;oldEmoInf=0.0f;
                currAnimName=PString("Walk");
                break;
    case KC_E : object->LoadAnimation(objectDB,"run_standard.kin");
                currEmoInf=1.0f;oldEmoInf=0.0f;
                currAnimName=PString("Run");
                break;
    case KC_R : object->LoadAnimation(objectDB,"walk_step_left.kin");
                currEmoInf=1.0f;oldEmoInf=0.0f;
                currAnimName=PString("Step Left");
                break;
    case KC_T : object->LoadAnimation(objectDB,"walk_step_right.kin");
                currEmoInf=1.0f;oldEmoInf=0.0f;
                currAnimName=PString("Step Right");
                break;

    case KC_Y : object->LoadAnimation(objectDB,"flap_like_a_bird.kin");
                currEmoInf=1.0f;oldEmoInf=0.0f;
                currAnimName=PString("Flap like bird");
                break;
    case KC_U : object->LoadAnimation(objectDB,"hand_on_heart.kin");
                currEmoInf=1.0f;oldEmoInf=0.0f;
                currAnimName=PString("Hand on Heart");
                break;


  }

  return Propagation::Handled;
}



//
// WinMain:
//
// Entry point to our example.
//
int APIENTRY WinMain(HINSTANCE instance, HINSTANCE prevInstance, char *cmdLine, int cmdShow)
{
  // Engine application object
  App theApp;

  // Create operating system specific class
  OS os(instance, cmdLine);

  // Create the application with specified name and operating
  // system specific data.
  if (!theApp.Init("EmotionDemo", &os))
  {
    LOGERR(("WinMain() Unable to initialize application object"));
    return -1;
  }

  // Create initial user task
  Task *task = NewMem(EmotionDemoTask);

  // Run main loop
  int result = theApp.Main();

  // Release user task
  DeleteMem(task);

  return result;
}


