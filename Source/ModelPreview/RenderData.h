/*--------------------------------------------------------------------
  RenderData.h

  Contains the definition a class used to contain the rendering 
  details.

  16/03/2001 -- Created the file

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#ifndef _INCLUDED_RENDER_DATA_H_
#define _INCLUDED_RENDER_DATA_H_

#include "stdafx.h"
#include <PString.h>
#include <Vector3.h>

using namespace Jet;

enum MODEL_MOVE{

    ROTATE_ALL = 0,
    ROTATE_X   = 1,
    ROTATE_Y   = 2,
    ROTATE_Z   = 3,
};

class RenderData  
{
 


public:
	RenderData();
	virtual ~RenderData();


    //Model Data
    PString currModelName;
    MODEL_MOVE movement;
    int totPoly;
    int currPoly;
    int currDetailPercent;
    bool scaleDetail;
    bool orbit;

    //Camera data
    float cameraDist;

    //Animation
    PString currAnimationName;
    int  speed;
    bool reverse;

    //Skeleton vertex data
    PString vertexName;
    bool showVertex;
    bool selectVertex;
    int vertexRoll; 
    int vertexPitch; 
    int vertexYaw; 

    //Display data
    bool skeleton;
    bool wireframe;
    bool localaxis;
    bool modelDisplayed;
    int transparency;

};

#endif 
