/*--------------------------------------------------------------------
  Main.cpp

  Contains the starting point for the program.

  16/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

// startup support header files
#include "StdAfx.h"
#include <OS.h>
#include <App.h>
#include <Task.h>
#include <OpSysWindows.h>

#include "ModelPreviewTask.h"

using namespace Jet;

//
// Main Win32 application function
//
int APIENTRY WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
  // Create operating system specific class
  OS windows(NULL, cmdLine);

  // Create the application with specified name and operating
  // system specific data.
  App theApp;
  theApp.Init("ModelPreview", &windows);

  // create initial user task
  Task *userTask = NewMem(ModelPreviewTask);

  // run main loop
  int result = theApp.Main();

  // release user task
  DeleteMemZ(userTask);

  return result;
}


