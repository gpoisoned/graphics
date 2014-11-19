#include "utility.h"

void crossProduct(float *u, float *v, float *result){
	result[0] = u[1] * v[2] - u[2] * v[1]; // resultant x
	result[1] = u[2] * v[0] - u[0] * v[2]; // resultant y
	result[2] = u[0] * v[1] - u[1] * v[0]; // resultant z
}

// Draw Triangle
// TODO: Define Normal
void drawTriangle(float *v1, float *v2, float *v3, float *color){
	glBegin(GL_TRIANGLES);
	glColor3fv(color);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(v3);
	glEnd();
}

// Draw Torus
void torusVertex(float th, float ph, float R, float r){
	 float x = (R + r * Cos(ph)) * Cos (th);
	 float y = (R + r * Cos(ph)) * Sin(th);
	 float z = r * Sin(ph);

	 // Normal for Torus
	 glNormal3d(Cos (th) * Cos (ph), Sin(th) * Cos(ph), Sin(ph));
	 glVertex3f(x,y,z);
}

void drawTorus(float *pos, float *color, float theta, float phi, float R,
							 float r, int delta){
	 for (float th = 0; th <= theta; th += delta){
			glBegin(GL_QUAD_STRIP);
			glColor3fv(color);
			for (float ph = 0; ph <= phi; ph += delta){
					torusVertex(th, ph, R, r);
					torusVertex(th + delta, ph, R, r);
			}
			glEnd();
	 }
}

// Draw Cylinder
void drawCylinder(float theta, int delta, float *color, float Rtop, float Rbot){
	glBegin(GL_QUAD_STRIP);
	glColor3fv(color);
	for (float th = 0; th <= theta; th += delta){
			glNormal3f(Cos(th),0,Sin(th));
			glVertex3f(Rtop * Cos(th), +1, Rtop *Sin(th));
			glVertex3f(Rbot * Cos(th), -1, Rbot * Sin(th));
	}
	glEnd();
}

/*
 *  Draw vertex in polar coordinates with normal
 */
void Vertex(float th,float ph)
{
	 float x = Sin(th)*Cos(ph);
	 float y = Cos(th)*Cos(ph);
	 float z =         Sin(ph);
	 //  For a sphere at the origin, the position
	 //  and normal vectors are the same
	 glNormal3d(x,y,z);
	 glVertex3d(x,y,z);
}

void drawSphere(int delta, float scale, float x, float y, float z){
	glPushMatrix();
	glTranslated(x,y,z);
	glScaled(scale, scale, scale);
	 for (int ph=-90; ph <= 90; ph += delta){
			glBegin(GL_QUAD_STRIP);
			for (int th=0; th<=360; th+=2*delta)
			{
				 Vertex(th, ph);
				 Vertex(th, ph + delta);
			}
			glEnd();
	 }
	glPopMatrix();
}

void drawCircle(float R, float theta, int delta, float *color){
	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(color);
	glVertex3f(0.0, 0.0, 0.0);
	glNormal3f(0.0, -1.0, 0.0);
	for (float th = 0; th <= theta; th += delta){
		glVertex3f(R * Cos(th),0.0, R * Sin(th));
	}
	glEnd();
}

void drawSlantedSide(){
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0, -0.5, -1);
	for(float i=0; i< 4; i++){
		for(float j=0;j<5;j++){
			glVertex3d(1+(j-2)*(8-i)/16 ,0.25*i, i/8);
			glVertex3d(1+(j-2)*(7-i)/16 ,0.25*(i+1), (i+1)/8);
		}
	}
	glEnd();
}

void drawFourSlantedSides(float lengthScale, float widthScale, float *color){
	glPushMatrix();
	glColor3fv(color);
	glScaled(1, lengthScale, 1);
	glScaled(widthScale, 1, widthScale);
	drawSlantedSide();

	glTranslated(2,0,0);
	glRotated(-90,0,1,0);
	drawSlantedSide();

	glTranslated(2,0,0);
	glRotated(-90,0,1,0);
	drawSlantedSide();

	glTranslated(2,0,0);
	glRotated(-90,0,1,0);
	drawSlantedSide();
	glPopMatrix();
}


// Draw cube with small quads on each side
void drawCubeSide(){
	int numParts = 100;
	float len = 1;
	float delta;
	delta = len / numParts;

	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.0, 0.0, -1.0);
	for(int ver = 0; ver <= numParts; ver++){
		for (int hor = 0; hor <= numParts; hor++){
			glVertex3f(hor*delta, ver *delta,0);
			glVertex3f(hor*delta, (ver+1) * delta,0);
		}
	}
	glEnd();
}

void drawCube(float *color, int closedTop, int closedBottom){
	glPushMatrix();
	glColor3fv(color);
		drawCubeSide();
		glTranslated(1,0,0);
		glRotated(-90,0,1,0);
		drawCubeSide();
		glTranslated(1,0,0);
		glRotated(-90,0,1,0);
		drawCubeSide();
		glTranslated(1,0,0);
		glRotated(-90,0,1,0);
		drawCubeSide();
	glPopMatrix();
		if (closedTop){
			glPushMatrix();
			glTranslated(0,1,0);
			glRotated(90,1,0,0);
			drawCubeSide();
			glPopMatrix();
		}
		if (closedBottom){
			glPushMatrix();
			glTranslated(0,0,1);
			glRotated(-90,1,0,0);
			drawCubeSide();
			glPopMatrix();
		}
}

void drawSemiCircularShape(float *color, int closedBottom){
		glPushMatrix();
			glPushMatrix();
				glTranslated(0,0,0.15);
				glRotated(-90,1,0,0);
				drawCircle(1,180,10,color);
			glPopMatrix();
			glPushMatrix();
				glTranslated(0,0,-0.15);
				glRotated(-90,1,0,0);
				glRotated(180,0,0,1);
				drawCircle(1,180,10,color);
			glPopMatrix();
			glPushMatrix();
				glScaled(1,1,0.15);
				glRotated(-90,1,0,0);
				drawCylinder(180, 10, color, 1, 1);
			glPopMatrix();

			if (closedBottom){
				// Patch the bottom with a side of a cube
				glPushMatrix();
					glColor3fv(color);
					glScaled(2,1,0.30);
					glTranslated(-0.5,0,-0.50);
					glRotated(90,1,0,0);
					drawCubeSide();
				glPopMatrix();
			}
		glPopMatrix();
}

//-----------------------------------------------------//
//-----------------------------------------------------//
// Objects here are more complex and built using the stuff
// defined above

void drawRoundTable(float scale, float *color)
{
	glPushMatrix();
	glScaled(scale, scale, scale);
		glPushMatrix();
			glScaled(1,0.10,1);
			drawCylinder(360, 10, color, 2.5, 2.5);
			glPushMatrix();
				glTranslated(0,1,0);
				drawCircle(2.5, 360, 10, color);
				glPushMatrix();
					glTranslated(0,-2,0);
					drawCircle(2.5, 360, 10, color);
				glPopMatrix();

			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glScaled(1,0.08,1);
			glTranslated(0,2.5,0);
			drawCylinder(360, 10, color, 2.40, 2.40);
			glPushMatrix();
				glTranslated(0,1,0);
				drawCircle(2.40, 360, 10, color);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glTranslated(-0.53, -0.80, -0.53);
			drawFourSlantedSides(0.80, 0.53, color);
		glPopMatrix();

		glPushMatrix();
			glScaled(1.05, 0.6, 1.05);
			glTranslated(-0.5, -2.35, -0.5);
			drawCube(color, false, false);
		glPopMatrix();

		//Base
		glPushMatrix();
			glScaled(1.05,0.15,1.05);
			glTranslated(-0.5, -10.4 ,-0.5);
			drawCube(color, false, true);
		glPopMatrix();

		glPushMatrix();
			glScaled(1.25,0.15,1.25);
			glTranslated(-0.5, -11.0 ,-0.5);
			drawCube(color, true, true);
		glPopMatrix();

		glPushMatrix();
			glScaled(1.35,0.15,1.35);
			glTranslated(-0.5, -12.0 ,-0.5);
			drawCube(color, true, true);
		glPopMatrix();

	glPopMatrix();
}

void drawSquareTable(float *color){
	glPushMatrix();
		// Base
		glPushMatrix();
		glScaled(1.5,0.3,1.25);
		drawCube(color, false, false);
		glPopMatrix();

		// Legs
		glPushMatrix();
		glScaled(0.25,2,0.25);
		glTranslated(-1 ,-0.85, 0);
		drawCube(color, false, false);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25,2,0.25);
		glTranslated(-1 ,-0.85, 4.0);
		drawCube(color, false, false);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25,2,0.25);
		glTranslated(6,-0.85, 0);
		drawCube(color, false, false);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25,2,0.25);
		glTranslated(6 ,-0.85, 4.0);
		drawCube(color, false, false);
		glPopMatrix();

		// Cover for the base
		glPushMatrix();
			glTranslated(-0.35, 0.325, -0.15);
			glScaled(2.20, 0.15, 1.5);
			drawCube(color, true, true);
		glPopMatrix();
	glPopMatrix();
}

void drawChair(float scale, float *color){
	glPushMatrix();
	glScaled(scale, scale, scale);

		// Build on top of table
		drawSquareTable(color);

		// Upper two bars
		glPushMatrix();
		glTranslated(-0.25 ,0.47, -0.15);
		glScaled(0.25,2,0.25);
		drawCube(color, true, false);
		glPopMatrix();

		glPushMatrix();
		glTranslated(1.5 ,0.47, -0.15);
		glScaled(0.25,2,0.25);
		drawCube(color, true, false);
		glPopMatrix();

		//Build the semi-circular part
		glPushMatrix();
		glTranslated(0.75,1.7,0);
		glScaled(0.85,1,1);
		drawSemiCircularShape(color, true);
		glPopMatrix();

		// Build the joints on the legs
		glPushMatrix();
		glScaled(0.25, 0.25, 0.75);
		glTranslated(-1.05,-4.0,0.34);
		drawCube(color,true, true);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.25, 0.75);
		glTranslated(6.0,-4.0,0.34);
		drawCube(color,true, true);
		glPopMatrix();
	glPopMatrix();
}

void drawTeaCup(float scale, float *color){
	glPushMatrix();
	glScaled(scale, scale, scale);
	// bottom cylinder
		glPushMatrix();
		glTranslated(0 ,1, 0);
		glScaled(1,1,1);
		drawCylinder(360, 10, color, 2.6, 2.2);
		glPopMatrix();
	// top cylinder
		glPushMatrix();
		glTranslated(0 ,3, 0);
		glScaled(1,1,1);
		drawCylinder(360, 10, color, 3.4, 3.0);
		glPopMatrix();

	// bottom circle	
		glPushMatrix();
		glTranslated(0 ,0, 0);
		glScaled(1,1,1);
		drawCircle(2.2, 360, 10, color);
		glPopMatrix();

	glPopMatrix();
}


//----------------------------------------------------//
/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
	 char    buf[LEN];
	 char*   ch=buf;
	 va_list args;
	 //  Turn the parameters into a character string
	 va_start(args,format);
	 vsnprintf(buf,LEN,format,args);
	 va_end(args);
	 //  Display the characters one at a time at the current raster position
	 while (*ch)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}
