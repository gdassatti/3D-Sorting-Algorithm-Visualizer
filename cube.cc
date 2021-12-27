//****************************************************//
// cube.cc ~ implementation of cube class             //
// Written by: Gavin Dassatti                         //
// Inspiration from: Professor Chelberg and his scene //
// scene class                                        //
// Last modified: 11-08-2021                          //
//                                                    //
// This file provides functionality for creating 3D   //
// cubes and generating them on the screen. I used    //
// this file from the scene-class-3D directory        //
// because it helped me get started with creating 3D  //
// cubes. I then modified it a bit to make it work    //
// with what I was trying to do.                      //
//****************************************************//
#include "cube.h"

bool cube::inited=false;
GLuint cube::NumPoints=36;


//----------------------------------------------------------------------------
// MyQuad generates two triangles for each face of the cube
void cube::MyQuad(int a, int b, int c, int d,  int i)
{
  vec4 vertices[8] = {
    vec4(-0.5, -0.5,  0.5, 1.0),
    vec4(-0.5,  0.5,  0.5, 1.0),
    vec4( 0.5,  0.5,  0.5, 1.0),
    vec4( 0.5, -0.5,  0.5, 1.0),
    vec4(-0.5, -0.5, -0.5, 1.0),
    vec4(-0.5,  0.5, -0.5, 1.0),
    vec4( 0.5,  0.5, -0.5, 1.0),
    vec4( 0.5, -0.5, -0.5, 1.0)
  };

  points[i] = vertices[a];
  points[i+1] = vertices[b];
  points[i+2] = vertices[c];

  points[i+3] = vertices[a];
  points[i+4] = vertices[c];
  points[i+5] = vertices[d];
}
//----------------------------------------------------------------------------



//-----------------------------------
// Simple default constructor
cube::cube() : Object()
{
  colorcube();
}
//-----------------------------------



//--------------------------------------------------
// colorcube generates 12 triangles and 36 vertices 
void cube::colorcube()
{
  if(!inited && points!=NULL){
    MyQuad(1, 0, 3, 2, 0);
    MyQuad(2, 3, 7, 6, 6);
    MyQuad(3, 0, 4, 7, 12);
    MyQuad(6, 5, 1, 2, 18);
    MyQuad(4, 5, 6, 7, 24);
    MyQuad(5, 4, 0, 1, 30);
  }
  inited = true;
}
//---------------------------------------------------



//---------------------------------------------------------------------
// Drawing function that takes an argument - which is
// used in the selection process
void cube::draw(bool select_mode)
{

  if(select_mode){
    glUniform4f(colorLoc, sr, sg, sb, 1.0);
  }else{
    glUniform4f(colorLoc, r, g, b, 1.0);
  }
  
  // Setting model view matrix and drawing cube in the correct position
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, index, NumPoints);
}
//----------------------------------------------------------------------



//-------------------------------------------------------------------------------------
// Initialization function.
cube::cube(vec4 *npoints, GLint nmodel_view, GLuint nindex, GLint ncolorLoc) : Object()
{
  
  x = y = z = 0.0;
  colorLoc = ncolorLoc;
  points=npoints;
  model_view=nmodel_view;
  index=nindex;
  colorcube();

}
//-------------------------------------------------------------------------------------
