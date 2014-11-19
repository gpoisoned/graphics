// Helper functions for OpenGL objects //

#ifndef utility_h
#define utility_h

// Required Header Files
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "math.h"

//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))
#define true 1
#define false 0

// Calculates cross product of vectors u and v in 3d space
// and stores the result in the final argument passed to the function
// Note: u, v, result are arrays of size 3
void crossProduct(float *u, float* v, float *result);

// Draw Triangle
void drawTriangle(float *v1, float *v2, float *v3, float *color);

// Draw Torus
void torusVertex(float th, float ph, float R, float r);
void drawTorus(float *pos, float *color, float theta, float phi, float R, float r,
								int delta);

// Draw cylinder
void drawCylinder(float theta, int delta, float *color, float Rtop, float Rbot);

// Draw sphere
void Vertex(float th,float ph);
void drawSphere(int delta, float scale, float x, float y, float z);

// Draw circle
void drawCircle(float R, float theta, int delta, float *color);

void drawSlantedSide();
void drawFourSlantedSides(float lengthScale, float widthScale, float *color);

// Draw cube with each side made of several quads
void drawCubeSide();
void drawCube(float *color, int closedTop, int closedBottom);

void drawSemiCircularShape(float *color, int closedBottom);

// ------ Bit complex stuff-----------//
void drawRoundTable(float scale, float *color);
void drawSquareTable(float *color);
void drawChair(float scale, float *color);
void drawTeaCup(float scale, float *color);

/* ********* Other Helpers*********** */
void Print(const char* format , ...);

#endif
