//****************************************************//
// cube.h  ~ header file of cube class                //
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
#ifndef cube_h
#define cube_h

#include <vector>
//#include "common.h"
#include "object.h"
using std::vector;

// Vertices of a unit cube centered at origin, sides aligned with axes
// extern vector<vec4 *> vertices;
extern vec4 vertices[8];

class cube : public Object{

public:

    static GLuint NumPoints;
    cube();
    void draw(bool select_mode = false);
    void MyQuad(int a, int b, int c, int d, int i);
    void colorcube();
    cube(vec4 *npoints, GLint nmodel_view, GLuint nindex, GLint ncolorLoc);
    void set_mv(mat4 nmv){
        mv=nmv;
    }
    float x, y, z;

private:

    mat4 mv;
    static bool inited;
    int Initial_Point;

};

#endif