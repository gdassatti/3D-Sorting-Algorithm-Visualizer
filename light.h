#ifndef LIGHT_H
#define LIGHT_H
//**********************************************************//
//                                                          //
//  Class definition of light object                        //
//                                                          //
//  Written by Prof. David M. Chelberg                      //
//                                                          //
//  Used By: Gavin Dassatti for Final Project               //
//  Light class implements common functions to all lights   //
//                                                          //
// last-modified: Fri Nov  3 14:48:45 2017                  //  
//                                                          //
//**********************************************************//

#include "Angel.h"
#include <iostream>
#include <stdlib.h>

class Light 
{
  public:

  // Default constructor
  Light();
  Light(vec4 position,
	vec3 diffuse,
	vec3 specular,
	vec3 ambient,
	bool drop_off=false,
	vec3 quadratic=vec3(1,0,0));

  // To set light properties
  // Where is the light source?
  void set_position(vec4 nposition){position=nposition;};

  void set_diffuse(vec3 ndiffuse){diffuse=ndiffuse;};
  void set_specular(vec3 nspecular){specular=nspecular;};
  void set_ambient(vec3 nambient){ambient=nambient;};
  void set_drop_off(bool ndrop_off){drop_off=ndrop_off;};
  void set_quadratic(vec3 nquadratic){quadratic=nquadratic;};

  // To retrieve light properties
  vec4 get_position(){return(position);};

  vec3 get_diffuse(){return(diffuse);};
  vec3 get_specular(){return(specular);};
  vec3 get_ambient(){return(ambient);};
  bool get_drop_off(){return(drop_off);};
  vec3 get_quadratic(){return(quadratic);};

private:
  vec4 position;
  vec3 diffuse;
  vec3 specular;
  vec3 ambient;
  // Whether the light intensity drops off
  bool drop_off;
  // Coeficients of the quadratic drop-off
  // 1/(quadratic.x + quadratic.y*distance + quadratic.z*distance^2)
  vec3 quadratic; 
};

#endif
