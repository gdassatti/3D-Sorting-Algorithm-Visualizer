// File: vshader.glsl
// Created By: Gavin Dassatti

attribute vec4 vPosition;
uniform vec4 vcolor;
varying vec4 fcolor;

uniform mat4 model_view;
uniform mat4 camera_view;
uniform mat4 projection;

void main() 
{
  
  gl_Position = projection*camera_view*model_view*vPosition;
  fcolor = vcolor;
  
}
