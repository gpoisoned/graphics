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

// Draw Torus
void torusVertex(float th, float ph, float R, float r);
void drawTorus(float *color, float theta, float phi, float R, float r,
								int delta);

// Draw cylinder
void drawCylinder(float theta, int delta, float *color, float Rtop, float Rbot, int texture);
void drawCylinderClosedEnds(float theta, int delta, float *color, float Rtop, float Rbot, int texture);

// Draw sphere
void Vertex(float th,float ph);
void drawSphere(int delta, float scale, float x, float y, float z);

// Draw circle
void drawCircle(float R, float theta, int delta, float *color, int texture);

void drawSlantedSide(int texture);
void drawFourSlantedSides(float lengthScale, float widthScale, float *color, int texture);

// Draw cube with each side made of several quads
void drawCubeSide(int texture);
void drawCube(float *color, int closedTop, int closedBottom, int texture);

void drawSemiCircularShape(float *color, int closedBottom, int texture);

// ------ Bit complex stuff-----------//
void drawRoundTable(float scale, float *color, int texture);
void drawSquareTable(float *color, int texture);
void drawChair(float scale, float *color, int texture);
void drawTeaCup(float scale, float *color, int texture);
void drawSofa(float scale, float *color, int texture);
void drawRoom(int wallTexture, int floorTexture, float D);
void drawShelf(float scale, float *color, int texture);
void drawTVstand(float scale, float *color, int texture);
void drawBowl(float scale, float *color, int texture);
void drawVase(float scale, float *color, int texture);
void drawBulb(float *color);
void drawLampUpright(float *color, int texture);
void drawLampDownFacing(float *color, int texture);
void drawTVScreen(float scale, float *color, int texture);
void drawTVFrame(float scale, float *color, int texture);

void drawDrumsSet(float *color, int cylinderTex, int circleTex, int metallicTex,
									int cymbalTex);
void drawDiningTable(float *color, int woodTex, int vaseTex);
void drawSofas(float *color, int sofaTex);
void drawTVWithStand(int frameTex, int screenTex, int shelfTex);
void drawTableWithLampStyle1(float *color, int tableTex, int lampTex);
void drawTableWithLampStyle2(float *color, int tableTex, int lampTex);

#endif
