//
// EmotionDemo.h
//

#ifndef _EMOTION_DEMO_TASK_H
#define _EMOTION_DEMO_TASK_H

// Jet Headers
#include <OS.h>
#include <Task.h>
#include <ResourceDatabase.h>
#include <RenderKernel.h>
#include <ParticleStructure.h>
#include <Camera.h>
#include <ViewPort.h>
#include <InputObserverSystem.h>
#include <UI\OSDConsole.h>

// Example object
#include "EmotionObject.h"

//
// All Jet classes are encapsulated in the 'Jet' namespace.  Before any Jet objects
// are used, the following namespace must be imported.
//
using namespace Jet;


//
// User task to perform initialization, updates and uninitialization.
// Also responds to user input.
//
class EmotionDemoTask : public Task, public InputObserver
{
public:

  EmotionDemoTask();

  //
  // Initialize the task. Return false upon critical failure.
  //
  virtual bool Init(OS *os);

  //
  // Clean up the task and release any allocated resources.
  //
  virtual void Done();

  //
  // Watch for user input to end program.
  //
  virtual UpdateCode Update(float dt);


  ///////////////////
  // Input Observer

  //
  // Called when a key is pressed/unpressed.
  // Return 'true' to prevent other observers from receiving this event.
  //
  virtual int OnKey(KeyEvent key);


protected:

  InputObserverSystemRef input;                   // Input system
  RenderKernel      *renderKernel;                // The render kernel forms the basis for all 2D and 3D rendering
  ResourceDatabase  *objectDB;                    // All sample objects are in this database.
  ResourceDatabase  *interfaceDB;                 // Interface components are in this database.
  Camera            *camera;                      // Our eye into the virtual world.
  ViewPort          *viewPort;                    // The 2d region in the window client area that
                                                  // we display something in.
  ParticleStructure *root;                        // The root pointer to our world.
  EmotionObject     *object;                      // The object loaded into our world.
  OSDConsole        *console;                     // Console for notification messages
  bool               done;                        // Flag to signify user termination.
  float              lastConsoleUpdate;           // Time since the last console update

  unsigned int emotionNum;
  float currEmoInf,oldEmoInf;
  PString currEmotionName,currAnimName;

  //
  // Do the initial work in setting up a functional virtual 3d world using JET.
  // Return false on failure.
  //
  bool CreateWorld(OS *os);

  //
  // Create the user interface objects. Return false on failure.
  //
  bool InitInterface(OS *os);

  //
  // Initialize all the resource databases.
  //
  void InitDatabases();
};

#endif // !__EmotionDemoTask_H
