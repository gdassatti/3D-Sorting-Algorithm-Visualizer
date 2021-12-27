//**********************************************************//
//                                                          //
//  Class implementation of light object                    //
//                                                          //
//  Written by Prof. David M. Chelberg                      //
//                                                          //
//  Used By: Gavin Dassatti for Final Project               //
//  Light class implements common functions to all lights   //
//                                                          //
// last-modified: Fri Nov  3 14:48:45 2017                  //  
//                                                          //
//**********************************************************//
#include "light.h"

// Default constructior
Light::Light()
{
  position=vec4(0,0,0,1);
  diffuse=vec3(1,1,1);
  specular=vec3(0,0,0);
  ambient=vec3(0.1, 0.1, 0.1);
  drop_off=false;
  quadratic=vec3(1,0,0);
}

Light::Light(vec4 nposition,
	     vec3 ndiffuse,
	     vec3 nspecular,
	     vec3 nambient,
	     bool ndrop_off,
	     vec3 nquadratic):position(nposition),
				    diffuse(ndiffuse),
				    specular(nspecular),
				    ambient(nambient),
				    drop_off(ndrop_off),
				    quadratic(nquadratic)
				     
{}
