/*--------------------------------------------------------------------
  ECanvas.cpp

  Contains the emotion canvas.

  08/06/2001 -- Created the file

This file has been hacked together quickly, don't expect good code

By Damian Trebilco
   Copyright © 2001. All Rights Reserved.
--------------------------------------------------------------------*/

#include "stdafx.h"
#include "ECanvas.h"

LOGCLIENT(ECanvas);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ECanvas::ECanvas():
Overlay("ECanvas",6,true)
{
    SetViewPortMode(true);
    influence=0.5f;
    emotion=0.0f;
    selectChar=false;
    //visible=true;
}

ECanvas::~ECanvas()
{

}

//Load the canvas
bool ECanvas::Init(){

  Display *display = renderKernel->GetDisplay();  
  
  // Load a texture.
  Bitmap image;
  if (!image.LoadAlpha("Emotion.bmp","Emotion_a.bmp"))
  {
    LOGERR(("ECanvas::Init() Unable to load image.bmp"));
    return false;
  }

  // Create a texture object to use when drawing our interface. Rand
  texture1 = display->CreateTexture();

  // Upload the image to the texture object.
  DisplayFormat format1(RGBA8888);
  texture1->Load(TEXTURE_STATIC, image, format1,false,false);

  //Load the indicator texture
  Bitmap image2;
  if (!image2.LoadAlpha("indicate.bmp","indicate_a.bmp"))
  {
    LOGERR(("ECanvas::Init() Unable to load indicate.bmp"));
    return false;
  }
  // Create a texture object to use when drawing our interface.
  texture2 = display->CreateTexture();

  // Upload the image to the texture object.
  DisplayFormat format2(RGBA8888);
  texture2->Load(TEXTURE_STATIC, image2, format2);

  //Load the 2nd indicator texture
  Bitmap image3;
  if (!image3.LoadAlpha("indicate2.bmp","indicate_a.bmp"))
  {
    LOGERR(("ECanvas::Init() Unable to load indicate2.bmp"));
    return false;
  }
  // Create a texture object to use when drawing our interface.
  texture3 = display->CreateTexture();

  // Upload the image to the texture object.
  DisplayFormat format3(RGBA8888);
  texture3->Load(TEXTURE_STATIC, image3, format3);

  
  // Setup the first vertex.
  vertexArray[0].x = 0.0f;
  vertexArray[0].y = 0.5f;
  vertexArray[0].z = 0.0f;

  vertexArray[0].u = 0.0f;
  vertexArray[0].v = 1.0f;

  vertexArray[0].color = display->ConvertColor(Color(1.0f, 1.0f, 1.0f));

  vertexArray[1].x = 0.0f;  
  vertexArray[1].y = 0.0f;
  vertexArray[1].z = 0.0f;

  vertexArray[1].u = 0.0f;  
  vertexArray[1].v = 0.0f;

  vertexArray[1].color = display->ConvertColor(Color(1.0f, 1.0f, 1.0f));


  // Setup the third vertex.
  vertexArray[2].x = 0.4f;
  vertexArray[2].y = 0.5f;
  vertexArray[2].z = 0.0f;

  vertexArray[2].u = 1.0f;
  vertexArray[2].v = 1.0f;

  vertexArray[2].color = display->ConvertColor(Color(1.0f, 1.0f, 1.0f));

  // Setup the fourth vertex.
  vertexArray[3].x = 0.4f;
  vertexArray[3].y = 0.0f;
  vertexArray[3].z = 0.0f;

  vertexArray[3].u = 1.0f;
  vertexArray[3].v = 0.0f;
  vertexArray[3].color = display->ConvertColor(Color(1.0f, 1.0f, 1.0f));
   


 return true;
}

void ECanvas::Render(){
  
  Display *display = renderKernel->GetDisplay();


  // Set the active texture to the console texture.
  texture1->Activate();      

  
  //display->SetBlendFunction(Src::ONE, Dst::ZERO);
  display->SetBlendFunction(Src::SRC_ALPHA,Dst::ONE_MINUS_SRC_ALPHA);  
  //Draw the console
  display->DrawArray(D_TRIANGLE_STRIP, vertexArray, 4);
  

  // Set the active texture to the indicator texture.
  display->SetBlendFunction(Src::SRC_ALPHA,Dst::ONE_MINUS_SRC_ALPHA);  
  Matrix4 mat4;

  //Draw the indicator for world and character
  mat4.Identity();
  texture3->Activate();      
  mat4.Translate(((461.0f/512.0f)*0.4f),   
                 ((70.0f/512.0f)*0.5f),0.0f);
  mat4.Scale(0.06f,0.06f,0.06f);
  mat4.Translate(-0.2f,-0.2f,0.0f);
  display->LoadMatrix(mat4);
  display->DrawArray(D_TRIANGLE_STRIP, vertexArray, 4);

  mat4.Identity();
  texture2->Activate();      
  mat4.Translate(((147.0f/512.0f)*0.4f),   
                 ((70.0f/512.0f)*0.5f),0.0f);
  mat4.Scale(0.06f,0.06f,0.06f);
  mat4.Translate(-0.2f,-0.2f,0.0f);
  display->LoadMatrix(mat4);
  display->DrawArray(D_TRIANGLE_STRIP, vertexArray, 4);



  //Draw all the main indicators
  texture2->Activate();      
  mat4.Identity();
  mat4.Translate(cosf(mainEmotion*(PI/180.0f))*(117.0f/512.0f)*0.4f +((238.0f/512.0f)*0.4f),   
                 sinf(mainEmotion*(PI/180.0f))*(117.0f/512.0f)*-0.5f+((348.0f/512.0f)*0.5f),0.0f);
  mat4.Scale(0.06f,0.06f,0.06f);
  mat4.Translate(-0.2f,-0.2f,0.0f);
  display->LoadMatrix(mat4);

  //Draw the 1st indicator
  display->DrawArray(D_TRIANGLE_STRIP, vertexArray, 4);

  mat4.Identity();
  mat4.Translate(mainInfluence *(382.0f/512.0f)*0.4f +((107.0f/512.0f)*0.4f),   
                 ((143.0f/512.0f)*0.5f),0.0f);
  mat4.Scale(0.06f,0.06f,0.06f);
  mat4.Translate(-0.2f,-0.2f,0.0f);
  display->LoadMatrix(mat4);

  //Draw the 2nd indicator
  display->DrawArray(D_TRIANGLE_STRIP, vertexArray, 4);

  if(selectChar){
      //Draw the character indicators
      texture3->Activate();      
      mat4.Identity();
      mat4.Translate(cosf(emotion*(PI/180.0f))*(117.0f/512.0f)*0.4f +((238.0f/512.0f)*0.4f),   
                     sinf(emotion*(PI/180.0f))*(117.0f/512.0f)*-0.5f+((348.0f/512.0f)*0.5f),0.0f);
      mat4.Scale(0.06f,0.06f,0.06f);
      mat4.Translate(-0.2f,-0.2f,0.0f);
      display->LoadMatrix(mat4);

      //Draw the 1st indicator
      display->DrawArray(D_TRIANGLE_STRIP, vertexArray, 4);

      mat4.Identity();
      mat4.Translate(influence *(382.0f/512.0f)*0.4f +((107.0f/512.0f)*0.4f),   
                     ((143.0f/512.0f)*0.5f),0.0f);
      mat4.Scale(0.06f,0.06f,0.06f);
      mat4.Translate(-0.2f,-0.2f,0.0f);
      display->LoadMatrix(mat4);

      //Draw the 2nd indicator
      display->DrawArray(D_TRIANGLE_STRIP, vertexArray, 4);
  }


  display->SetBlendFunction(Src::ONE, Dst::ZERO);

}


void ECanvas::SetEmotion(float emo){
  emotion=emo;
 return;
}

void ECanvas::SetInfluence(float inf){
  influence=inf;
 return;
}


void ECanvas::SetMainEmotion(float emo){
  mainEmotion=emo;
 return;
}

void ECanvas::SetMainInfluence(float inf){
  mainInfluence=inf;
 return;
}


void ECanvas::Setworldchar(bool wc){
  selectChar=wc;
 return;
}