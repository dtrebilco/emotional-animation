//
// VE.h
//
// This example demonstrates using the motion interface.
//

#ifndef _INCLUDED_VE_H
#define _INCLUDED_VE_H

// startup support header files
#include <OS.h>
#include <Task.h>

// input support header files
#include <InputObserverSystem.h>
#include <InputObserver.h>

// render support header files
#include <Camera.h>
#include <RenderKernel.h>
#include <ParticleStructure.h>
#include <ResourceDatabase.h>
#include <Viewport.h>
#include <GeometryBaseInstance.h>

// skeleton movement blending header files
#include <SkeletonModifier.h>

// animation header files
#include <Animator.h>

// movement system header files
#include <Movement.h>
#include <MovementObserver.h>
#include <MotionEventType.h>
#include <MovieCanvas.h>
#include <FastDynamicArray.h>
#include <Rand.h>
#include <Modifier.h>
#include <Overlay.h>
#include <IndexedMeshInstance.h>


class VETask;
class VECharacter;

#include "EmotionSkeletonModifier.h"
#include "VEInferenceEngine.h"
#include "ECanvas.h"

//
// All Jet classes are encapsulated in the 'Jet' namespace.  Before any Jet objects
// are used, the following namespace must be imported.
//
using namespace Jet;

//
// VECharacter Class
//
// Object that will load a mesh with an animation and movement interface
//
class VECharacter: public ParticleStructure, public Modifier//, public AnimationObserver
{
public:

  //
  // Constructor/destructor
  //
  VECharacter();
  ~VECharacter();

  //
  // Load character and attach animation and movement interfaces
  //
  bool Init(VETask *vet,RenderKernel *renderKernel, GeometryBaseInstance & geometry,const ResourceDatabase *database);

  //
  // Move the character to the 'destination' location.  
  // Limit speed as required
  //
  bool MoveTo(float x, float y, float speed = -1.0f);


  ////////////////////
  // MovementObserver

  //
  // This method is called when a motion event is triggered.
  //
  //void OnEvent(MotionEventType type);

  //
  // Update
  //
  // Update the particle structure for the time  increment 'dt'
  //
  virtual Modifier::Result Update(float dt);
  //void JETAPI ResetBoundingVolume();
  //void JETAPI Cull(ViewPort &view, const Matrix4 &trans);

  //virtual void OnEvent(AnimationEvent &animEvent);
  bool SetBV(Particle *p);
  void SetSpeed(float s);
  Vector2 GetTarget();

  EmotionSkeletonModifier* GetEmotionSkeletonModifier();



  void Select();
  void UnSelect();
protected:

  ///////////////////
  // Data members

  EmotionSkeletonModifier *skeleton;      // skeleton to blend animation and movement
  Animator                *animator;      // animation interface
  VETask                  *virtET;        // Virtual environment
  //EmotionModifier         *VEEmotion;
  RenderKernel            *renderKernel;

  Vector2 currTarget,currPos;
  float speed;
};


//
// VETask
//
class VETask : public Task, public InputObserver
{
public:

  VETask();

  //
  // Initialize the task. Return false upon critical failure.
  //
  virtual bool Init(OS *os);

  //
  // Clean up the task. Should be able to handle not being initialized.
  //
  virtual void Done();

  //
  // The update method is called by the TaskManager with an amount of
  // time for the task to resynchronize to.  Overload this Method to
  // perform your processing and return CONTINUE, STOP to stop this
  // task from updating or EXIT to quit the from the TaskManager.
  //
  virtual UpdateCode Update(float dt);


  //
  // Create our rendering framework
  // return false on failure
  //
  bool CreateWorld(OS *os);
  
  //
  // Create and initialize the input system
  //
  bool CreateInputSystem(OS *os);

  //
  // Load a 3d object
  //
  ParticleStructure *Load3dObject(ResourceDatabase *database, const char *name);

  //
  // SetCursorPosition
  //
  bool SetCursorPosition(const Point &pos);

  //
  // Scroll the camera in the given direction, but only when the camera remains in 
  // its movement ranges.
  //
  bool Scroll(int dx, int dy);

  //
  // Zoom the camera in and out by the specified distance.
  //
  void Zoom(float delta);

  //
  // Check distance of the camera from terrain
  //
  void AdjustCameraPosition();

  //
  // Convert 2d screen position to the 3d position
  //
  Vector3 ScrPositionTo3d(const Point &pos, float z);

  //
  // Get the height of the map at a particular point.
  //
  float GetHeightAt(float x, float y);


  /////////////////
  // InputObserver

  //
  // Called when a key is pressed/unpressed.
  // Return 'true' to prevent other observers from receiving this event.
  //
  virtual int OnKey(KeyEvent key);

  //
  // Called when the mouse position changes.
  // Return 'true' to prevent other observers from receiving this event.
  //
  virtual int OnMouse(MouseEvent mouse, const Point &pos);


  void UpdateCollision(VECharacter *c);
private:
  bool AddObjects();
  bool AddCharacters();
protected:

  RenderKernel          *renderKernel;  // The render kernel forms the basis for all 2D and 3D rendering
  Camera                *camera;        // Our eye into the virtual world.
  ViewPort              *viewPort;      // The 2d region in the window client area that
                                        // we display something in.
  ParticleStructure     *root;          // The base of our world.
  InputObserverSystemRef input;         // Input system

  Size                   windowSize;    // Window size
  Point                  mouPosition;   // Position of the mouse
  int                    scrollDX;
  int                    scrollDY;

  ResourceDatabase      *terrainDB;     // All our sample files are in these databases.
  ResourceDatabase      *characterDB;
  ResourceDatabase      *cursorDB;
  ResourceDatabase      *objectDB;

  ParticleStructure     *terrain;       // Our objects
  ParticleStructure     *cursor;

  ECanvas               *emotionControl;  //The control
  float                 emotion,influence;
  int                   selCharacterIndex;

  FastDynamicArray<VECharacter  *>      characters; //Added objects
  FastDynamicArray<ParticleStructure *> objectList;

  VEInferenceEngine *emotionInference;

  bool                   done;
};


#endif // !__VE_H
