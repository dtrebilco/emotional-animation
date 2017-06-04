/*--------------------------------------------------------------------
  RenderData.cpp

  Contains the definition a class used to contain the rendering 
  details.

  16/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/
#include "stdafx.h"
#include "RenderData.h"

using namespace Jet;


/*-------------------------------------------------------------------
PROCEDURE: RenderData
PURPOSE  : To construct the object
PRE : TRUE
POST: The object has been constructed.
-------------------------------------------------------------------*/
RenderData::RenderData():
movement(ROTATE_ALL),
currModelName("None"),
totPoly(0),
currPoly(0),
currDetailPercent(100),
scaleDetail(false),
orbit(false),
cameraDist(1.0f),
currAnimationName("None"),
speed(100),
reverse(false),
skeleton(false),
wireframe(false),
localaxis(false),
modelDisplayed(true),
transparency(100),
vertexName("None"),
showVertex(false),
selectVertex(false),
vertexRoll(0),
vertexPitch(0),
vertexYaw(0)
{

}
/*-------------------------------------------------------------------
PROCEDURE: ~RenderData
PURPOSE  : To destroy the object
PRE : TRUE
POST: The object has been destroyed.
-------------------------------------------------------------------*/
RenderData::~RenderData()
{

}
