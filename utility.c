#include "utility.h"

void crossProduct(float *u, float *v, float *result){
	result[0] = u[1] * v[2] - u[2] * v[1]; // resultant x
	result[1] = u[2] * v[0] - u[0] * v[2]; // resultant y
	result[2] = u[0] * v[1] - u[1] * v[0]; // resultant z
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

void drawTorus(float *color, float theta, float phi, float R, float r, int delta){
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
void drawCylinder(float theta, int delta, float *color, float Rtop, float Rbot, int texture){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUAD_STRIP);
	glColor3fv(color);
	for (float th = 0; th <= theta; th += delta){
			glNormal3f(Cos(th),0,Sin(th));
			glTexCoord2f(Cos(th),1);
			glVertex3f(Rtop * Cos(th), +1, Rtop * Sin(th));
			glTexCoord2f(Cos(th),0);
			glVertex3f(Rbot * Cos(th), -1, Rbot * Sin(th));
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawCylinderClosedEnds(float theta, int delta, float *color, float Rtop, float Rbot, int texture){
	glPushMatrix();
		drawCylinder(theta, delta, color, Rtop, Rbot, texture);
		glPushMatrix();
			glTranslated(0, 1, 0);
			drawCircle(Rtop, theta, delta, color, texture);
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, -1, 0);
			glRotated(180,1,0,0);
			drawCircle(Rtop, theta, delta, color, texture);
		glPopMatrix();
	glPopMatrix();
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

void findNormal(float *v1, float *v2, float *result){
	float temp[3];
	float len;
	temp[0] = v1[0] - v2[0];
	temp[1] = v1[1] - v2[1];
	temp[2] = v1[2] - v2[2];
	len = sqrt((temp[0]*temp[0]) + (temp[1] * temp[1]) + (temp[2]* temp[2]));
	result[0] = temp[0] / len;
	result[1] = temp[1] / len;
	result[2] = temp[2] / len;
}

void drawCircle(float R, float theta, int delta, float *color, int texture){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(color);
	glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.5,0.5);
	glNormal3f(0.0, 1.0, 0.0);
	for (float th = 0; th <= theta; th += delta){
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.5*Cos(th)+0.5, 0.5*Sin(th)+0.5);
		glVertex3f(R * Cos(th),0.0, R * Sin(th));
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawSlantedSide(int texture){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0, -0.5, -1);
	for(float i=0; i< 4; i++){
		for(float j=0;j<5;j++){
			glTexCoord2f(i/4, j/5);
			glVertex3d(1+(j-2)*(8-i)/16 ,0.25*i, i/8);
			glTexCoord2f(i/4 + 1, (j+1)/5);
			glVertex3d(1+(j-2)*(7-i)/16 ,0.25*(i+1), (i+1)/8);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawFourSlantedSides(float lengthScale, float widthScale, float *color, int texture){
	glPushMatrix();
	glColor3fv(color);
	glScaled(1, lengthScale, 1);
	glScaled(widthScale, 1, widthScale);
	drawSlantedSide(texture);

	glTranslated(2,0,0);
	glRotated(-90,0,1,0);
	drawSlantedSide(texture);

	glTranslated(2,0,0);
	glRotated(-90,0,1,0);
	drawSlantedSide(texture);

	glTranslated(2,0,0);
	glRotated(-90,0,1,0);
	drawSlantedSide(texture);
	glPopMatrix();
}


// Draw cube with small quads on each side
void drawCubeSide(int texture){
	int numParts = 100;
	float len = 1;
	float delta;
	delta = len / numParts;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.0, 0.0, -1.0);
	for(int ver = 0; ver <= numParts; ver++){
		for (int hor = 0; hor <= numParts; hor++){
			glTexCoord2f(hor*delta, ver*delta);
			glVertex3f(hor*delta, ver *delta,0);
			glTexCoord2f(hor*delta, (ver+1)*delta);
			glVertex3f(hor*delta, (ver+1) * delta,0);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawCube(float *color, int closedTop, int closedBottom, int texture){
	glPushMatrix();
	glColor3fv(color);
		drawCubeSide(texture);
		glTranslated(1,0,0);
		glRotated(-90,0,1,0);
		drawCubeSide(texture);
		glTranslated(1,0,0);
		glRotated(-90,0,1,0);
		drawCubeSide(texture);
		glTranslated(1,0,0);
		glRotated(-90,0,1,0);
		drawCubeSide(texture);
	glPopMatrix();
		if (closedTop){
			glPushMatrix();
			glTranslated(0,1,0);
			glRotated(90,1,0,0);
			drawCubeSide(texture);
			glPopMatrix();
		}
		if (closedBottom){
			glPushMatrix();
			glTranslated(0,0,1);
			glRotated(-90,1,0,0);
			drawCubeSide(texture);
			glPopMatrix();
		}
}

void drawSemiCircularShape(float *color, int closedBottom, int texture){
		glPushMatrix();
			glPushMatrix();
				glTranslated(0,0,0.15);
				glRotated(-90,1,0,0);
				drawCircle(1,180,10,color, texture);
			glPopMatrix();
			glPushMatrix();
				glTranslated(0,0,-0.15);
				glRotated(-90,1,0,0);
				glRotated(180,0,0,1);
				drawCircle(1,180,10,color, texture);
			glPopMatrix();
			glPushMatrix();
				glScaled(1,1,0.15);
				glRotated(-90,1,0,0);
				drawCylinder(180, 10, color, 1, 1, texture);
			glPopMatrix();

			if (closedBottom){
				// Patch the bottom with a side of a cube
				glPushMatrix();
					glColor3fv(color);
					glScaled(2,1,0.30);
					glTranslated(-0.5,0,-0.50);
					glRotated(90,1,0,0);
					drawCubeSide(texture);
				glPopMatrix();
			}
		glPopMatrix();
}

//-----------------------------------------------------//
//-----------------------------------------------------//
// Objects here are more complex and built using the stuff
// defined above

void drawRoundTable(float scale, float *color, int texture)
{
	glPushMatrix();
	glScaled(scale, scale, scale);
		glPushMatrix();
			glScaled(1,0.10,1);
			drawCylinder(360, 10, color, 2.5, 2.5, texture);
			glPushMatrix();
				glTranslated(0,1,0);
				drawCircle(2.5, 360, 10, color, texture);
				glPushMatrix();
					glTranslated(0,-2,0);
					drawCircle(2.5, 360, 10, color, texture);
				glPopMatrix();

			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glScaled(1,0.08,1);
			glTranslated(0,2.5,0);
			drawCylinder(360, 10, color, 2.40, 2.40, texture);
			glPushMatrix();
				glTranslated(0,1,0);
				drawCircle(2.40, 360, 10, color, texture);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glTranslated(-0.53, -0.80, -0.53);
			drawFourSlantedSides(0.80, 0.53, color, texture);
		glPopMatrix();

		glPushMatrix();
			glScaled(1.05, 0.6, 1.05);
			glTranslated(-0.5, -2.35, -0.5);
			drawCube(color, false, false, texture);
		glPopMatrix();

		//Base
		glPushMatrix();
			glScaled(1.05,0.15,1.05);
			glTranslated(-0.5, -10.4 ,-0.5);
			drawCube(color, false, true, texture);
		glPopMatrix();

		glPushMatrix();
			glScaled(1.25,0.15,1.25);
			glTranslated(-0.5, -11.0 ,-0.5);
			drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
			glScaled(1.35,0.15,1.35);
			glTranslated(-0.5, -12.0 ,-0.5);
			drawCube(color, true, true, texture);
		glPopMatrix();

	glPopMatrix();
}

void drawSquareTable(float *color, int texture){
	glPushMatrix();
		// Base
		glPushMatrix();
		glScaled(1.5,0.3,1.25);
		drawCube(color, false, false, texture);
		glPopMatrix();

		// Legs
		glPushMatrix();
		glScaled(0.25,2,0.25);
		glTranslated(-1 ,-0.85, 0);
		drawCube(color, false, false, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25,2,0.25);
		glTranslated(-1 ,-0.85, 4.0);
		drawCube(color, false, false, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25,2,0.25);
		glTranslated(6,-0.85, 0);
		drawCube(color, false, false, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25,2,0.25);
		glTranslated(6 ,-0.85, 4.0);
		drawCube(color, false, false, texture);
		glPopMatrix();

		// Cover for the base
		glPushMatrix();
			glTranslated(-0.35, 0.325, -0.15);
			glScaled(2.20, 0.15, 1.5);
			drawCube(color, true, true, texture);
		glPopMatrix();
	glPopMatrix();
}

void drawChair(float scale, float *color, int texture){
	glPushMatrix();
	glScaled(scale, scale, scale);

		// Build on top of table
		drawSquareTable(color, texture);

		// Upper two bars
		glPushMatrix();
		glTranslated(-0.25 ,0.47, -0.15);
		glScaled(0.25,2,0.25);
		drawCube(color, true, false, texture);
		glPopMatrix();

		glPushMatrix();
		glTranslated(1.5 ,0.47, -0.15);
		glScaled(0.25,2,0.25);
		drawCube(color, true, false, texture);
		glPopMatrix();

		//Build the semi-circular part
		glPushMatrix();
		glTranslated(0.75,1.7,0);
		glScaled(0.85,1,1);
		drawSemiCircularShape(color, true, texture);
		glPopMatrix();

		// Build the joints on the legs
		glPushMatrix();
		glScaled(0.25, 0.25, 0.75);
		glTranslated(-1.05,-4.0,0.34);
		drawCube(color,true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.25, 0.75);
		glTranslated(6.0,-4.0,0.34);
		drawCube(color,true, true, texture);
		glPopMatrix();
	glPopMatrix();
}

void drawTeaCup(float scale, float *color, int texture){
	// Save transformation
	glPushMatrix();
	// Offset, scale
	glScaled(scale, scale, scale);
	// top cylinder
		glPushMatrix();
		glTranslated(0 ,3.2, 0);
		glScaled(1,1,1);
		drawCylinder(360, 10, color, 3.4, 2.8, texture);
		glPopMatrix();

	// middle cylinder
		glPushMatrix();
		glTranslated(0 ,2, 0);
		glScaled(1,0.2,1);
		drawCylinder(360, 10, color, 2.8, 2.8, texture);
		glPopMatrix();

	// bottom cylinder
		glPushMatrix();
		glTranslated(0 ,1, 0);
		glScaled(1,1,1);
		drawCylinder(360, 10, color, 2.8, 2.4, texture);
		glPopMatrix();

	// bottom circle
		glPushMatrix();
		glTranslated(0 ,0, 0);
		glScaled(1,1,1);
		drawCircle(2.4, 360, 10, color, texture);
		glPopMatrix();

	// knob
		glPushMatrix();
		glTranslated(3 ,2, 0);
		glRotated(247, 0, 0, 1);
		glScaled(1,1,1);
		drawTorus(color, 200, 360, 1, 0.25, 1);
		glPopMatrix();

	glPopMatrix();
}

void drawSofa(float scale, float *color, int texture){
	// Save transformation
	glPushMatrix();
	// Offset, scale
	glScaled(scale, scale, scale);


	// bottom four legs
		glPushMatrix();
		glScaled(0.1, 0.2, 0.1);
		glTranslated(8, -1, 5);
		drawCube(color, false, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.1, 0.2, 0.1);
		glTranslated(-8, -1, 5);
		drawCube(color, false, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.1, 0.2, 0.1);
		glTranslated(8, -1, -5);
		drawCube(color, false, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.1, 0.2, 0.1);
		glTranslated(-8, -1, -5);
		drawCube(color, false, true, texture);
		glPopMatrix();

	// Two side frames
		glPushMatrix();
		glScaled(0.15, 0.5, 1.25);
		glTranslated(6, 0, -0.45);
		drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.15, 0.5, 1.25);
		glTranslated(-6, 0, -0.45);
		drawCube(color, true, true, texture);
		glPopMatrix();

	// Bottom frame
		glPushMatrix();
		glScaled(1.9, 0.15, 1.25);
		glTranslated(-0.46, 0, -0.45);
		drawCube(color, true, true, texture);
		glPopMatrix();

	// Back frame
		glPushMatrix();
		glScaled(1.69, 0.5, 0.2);
		glTranslated(-0.46, 0, -2.82);
		drawCube(color, true, true, texture);
		glPopMatrix();

	// Sofa cushion
		glPushMatrix();
		glScaled(0.75, 0.1, 1.25);
		glTranslated(-0.95, 1.5, -0.45);
		drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.75, 0.1, 1.25);
		glTranslated(0.15, 1.5, -0.45);
		drawCube(color, true, true, texture);
		glPopMatrix();

	glPopMatrix();

}

// This function could be used as a bookshelf when rotated vertically
void drawShelf(float scale, float *color, int texture){
	// Save transformation
	glPushMatrix();
	glRotated(-90, 1, 0,0);
	// Offset, scale
	glScaled(scale, scale, scale);
		// Left and right wall
		glPushMatrix();
		glScaled(0.1, 1.0, 2.0);
		glTranslated(16, 0, 0);
		drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.1, 1.0, 2.0);
		glTranslated(-16, 0, 0);
		drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.1, 1.0, 2.0);
		glTranslated(16, 0, -1);
		drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.1, 1.0, 2.0);
		glTranslated(-16, 0, -1);
		drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.1, 1.0, 2.0);
		glTranslated(-16, 0, -1);
		drawCube(color, true, true, texture);
		glPopMatrix();

		// Front wall (right)
		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(0, 0, 19.1);
		drawCube(color, true, true, texture);
		glPopMatrix();

		// Front wall (left)
		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(-1, 0, 19.1);
		drawCube(color, true, true, texture);
		glPopMatrix();

		// Back wall (right)
		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(0, 0, -20);
		drawCube(color, true, true, texture);
		glPopMatrix();

		// Back wall (Left)
		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(-1, 0, -20);
		drawCube(color, true, true, texture);
		glPopMatrix();

		// 2nd row
		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(0, 0, 10);
		drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(-1, 0, 10);
		drawCube(color, true, true, texture);
		glPopMatrix();

		// 3rd row
		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(0, 0, 0);
		drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(-1, 0, 0);
		drawCube(color, true, true, texture);
		glPopMatrix();

		// 4th row
		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(0, 0, -10);
		drawCube(color, true, true, texture);
		glPopMatrix();

		glPushMatrix();
		glScaled(1.6, 1.0, 0.1);
		glTranslated(-1, 0, -10);
		drawCube(color, true, true, texture);
		glPopMatrix();


	glPopMatrix();
}

void drawBulb(float *color){
	glColor3fv(color);
	glPushMatrix();
	glTranslated(0, 1, 0);

	//Draw the bottom cylinrical parts
	// Draw two cylinders and fill the gap with pipe shape
	int noTexture = -1; // Don't want to texture the light bulb

	drawCylinder(360, 5, color, 3, 3, noTexture);
	glPushMatrix();
		glTranslated(0,3,0);
		drawCylinder(360, 5, color, 3, 3, noTexture);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,1.5,0);
		glPushMatrix();
			glRotated(90,1,0,0);
			drawTorus(color, 360, 90, 3, 0.5, 10);
		glPopMatrix();

		glPushMatrix();
			glRotated(180,1,0,0);
			glRotated(90,1,0,0);
			drawTorus(color, 360, 90, 3, 0.5, 10);
		glPopMatrix();
	glPopMatrix();

	// Draw Spherical Ball on top of the stand
	glPushMatrix();
		drawSphere(5, 5, 0, 8, 0);
	glPopMatrix();

	glPopMatrix();
}

void drawLampBase(float *color, int texture){
	glColor3fv(color);
	glPushMatrix();
		// Draw Base
		glPushMatrix();
		glScaled(1, 0.25, 1);
		drawCylinder(360, 5, color, 4, 4, texture);
		// Close the top and bottom
		glTranslated(0,-0.50,0);
		drawCircle(4, 360, 5, color, texture);
		glTranslated(0,1.5,0);
		drawCircle(4, 360, 5, color, texture);
		glPopMatrix();

		// Draw the cylindrical rod
		glPushMatrix();
		glTranslated(0, 10 , 0);
		glScaled(0.5,10,0.5);
		drawCylinder(360, 5, color, 1, 1, texture);
		glPopMatrix();

	glPopMatrix();
}

void drawLampCover(float *color, float Rtop, float Rbot, int texture){
	glColor3fv(color);
	glPushMatrix();
		// Draw a cylinder with varying radius at the top and bottom
		glTranslated(0, 1, 0);
		glScaled(1,1.5,1);
		drawCylinder(360, 5, color, Rtop, Rbot, texture);

	glPopMatrix();
}

void drawLampUpright(float* color, int texture){
	glColor3fv(color);
	glPushMatrix();
		// Draw Lamp Base here
		drawLampBase(color, texture);
		// Draw a light bulb at the top of this stand
		glPushMatrix();
			glTranslated(0, 20, 0);
		 	glScaled(0.25, 0.25, 0.25);
			drawBulb(color);
		glPopMatrix();

		// Draw a cover for the lamp
		glTranslated(0, 21, 0);
		drawLampCover(color, 3, 8, texture);
	glPopMatrix();
}

void drawLampDownFacing(float *color, int texture){
	glColor3fv(color);
	glPushMatrix();
		// Daw Base
		drawLampBase(color, texture);

		// Draw the hand of the lamp
		glPushMatrix();
		  glTranslated(4.25, 22.25, 0);
		  glRotated(90, 0, 1, 0);
			glRotated(60, 1, 0, 0);
			// Make sure the width is same as the base
			glScaled(0.5, 5 ,0.5);
			drawCylinder(360, 5, color, 1, 1, texture);
		glPopMatrix();

		// Draw a small joint to attach a bulb
		glPushMatrix();
		  glTranslated(4.25 + 5, 20 + 4.5, 0);
		  glRotated(90, 0, 1, 0);
			glRotated(-30, 1, 0, 0);
			drawCylinder(360, 5, color, 1, 1, texture);
		glPopMatrix();

		// Attach a bulb
		glPushMatrix();
		  glTranslated(4.25 + 6, 20 + 3.25, 0);
		  glRotated(180,0, 0, 1);
			glRotated(25, 0, 0, 1);
			glScaled(0.25, 0.25, 0.25);
			drawBulb(color);
		glPopMatrix();

		// Draw Lamp Cover
		glPushMatrix();
		  glTranslated(4.25 + 6.5, 20 + 1.5, 0);
			glRotated(30, 0, 0, 1);
			drawLampCover(color, 1, 3, texture);
		glPopMatrix();
	glPopMatrix();
}

void drawRoom(int wallTexture, int floorTexture, float D){
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);

	//  Sides
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(-D,-D,-D);
	glTexCoord2f(1,0); glVertex3f(+D,-D,-D);
	glTexCoord2f(1,1); glVertex3f(+D,+D,-D);
	glTexCoord2f(0,1); glVertex3f(-D,+D,-D);

	glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
	glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
	glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
	glTexCoord2f(0,1); glVertex3f(+D,+D,-D);

	glTexCoord2f(0,0); glVertex3f(+D,-D,+D);
	glTexCoord2f(1,0); glVertex3f(-D,-D,+D);
	glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
	glTexCoord2f(0,1); glVertex3f(+D,+D,+D);

	glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
	glTexCoord2f(1,0); glVertex3f(-D,-D,-D);
	glTexCoord2f(1,1); glVertex3f(-D,+D,-D);
	glTexCoord2f(0,1); glVertex3f(-D,+D,+D);
	glEnd();

	//  Top and bottom
	glBindTexture(GL_TEXTURE_2D,floorTexture);
	glBegin(GL_QUADS);
	//glTexCoord2f(0,0);
	glVertex3f(+D,+D,-D);
	//glTexCoord2f(1,0);
	glVertex3f(+D,+D,+D);
	//glTexCoord2f(1,1);
	glVertex3f(-D,+D,+D);
	//glTexCoord2f(0,1);
	glVertex3f(-D,+D,-D);

	glTexCoord2f(0,0);glVertex3f(-D,-D,+D);
	glTexCoord2f(1,0);glVertex3f(+D,-D,+D);
	glTexCoord2f(1,1);glVertex3f(+D,-D,-D);
	glTexCoord2f(0,1);glVertex3f(-D,-D,-D);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawTVstand(float scale, float *color, int texture){
    // Save transformation
	glPushMatrix();
	glRotated(-90, 1, 0,0);
	// Offset, scale
	glScaled(scale, scale, scale);
    // Left and right wall
    glPushMatrix();
    glScaled(0.1, 1.0, 2.0);
    glTranslated(16, 0, 0);
    drawCube(color, true, true, texture);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.1, 1.0, 2.0);
    glTranslated(8, 0, 0);
    drawCube(color, true, true, texture);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.1, 1.0, 2.0);
    glTranslated(0, 0, 0);
    drawCube(color, true, true, texture);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.1, 1.0, 2.0);
    glTranslated(-8, 0, 0);
    drawCube(color, true, true, texture);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.1, 1.0, 2.0);
    glTranslated(-16, 0, 0);
    drawCube(color, true, true, texture);
    glPopMatrix();

    // Front wall (right)
    glPushMatrix();
    glScaled(1.6, 1.0, 0.1);
    glTranslated(0, 0, 19.1);
    drawCube(color, true, true, texture);
    glPopMatrix();

    // Front wall (left)
    glPushMatrix();
    glScaled(1.6, 1.0, 0.1);
    glTranslated(-1, 0, 19.1);
    drawCube(color, true, true, texture);
    glPopMatrix();

    // 2nd row
    glPushMatrix();
    glScaled(1.6, 1.0, 0.1);
    glTranslated(0, 0, 10);
    drawCube(color, true, true, texture);
    glPopMatrix();

    glPushMatrix();
    glScaled(1.6, 1.0, 0.1);
    glTranslated(-1, 0, 10);
    drawCube(color, true, true, texture);
    glPopMatrix();

    // 3rd row
    glPushMatrix();
    glScaled(1.6, 1.0, 0.1);
    glTranslated(0, 0, 0);
    drawCube(color, true, true, texture);
    glPopMatrix();

    glPushMatrix();
    glScaled(1.6, 1.0, 0.1);
    glTranslated(-1, 0, 0);
    drawCube(color, true, true, texture);
    glPopMatrix();


	glPopMatrix();

}

void drawBowl(float scale, float *color, int texture){
    // Save transformation
	glPushMatrix();
	// Offset, scale
	glScaled(scale, scale, scale);

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, -1, 0);
	drawCircle(0.5, 360, 10, color, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 0, 0);
	drawCylinder(360, 10, color, 0.7, 0.5, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 2, 0);
	drawCylinder(360, 10, color, 0.8, 0.7, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 4, 0);
	drawCylinder(360, 10, color, 0.85, 0.8, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 6, 0);
	drawCylinder(360, 10, color, 0.875, 0.85, texture);
	glPopMatrix();

	glPopMatrix();
}

void drawVase(float scale, float *color, int texture){
    // Save transformation
	glPushMatrix();
	// Offset, scale
	glScaled(scale, scale, scale);

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, -1, 0);
	drawCircle(0.5, 360, 10, color, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 0, 0);
	drawCylinder(360, 10, color, 0.7, 0.5, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 2, 0);
	drawCylinder(360, 10, color, 0.8, 0.7, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 4, 0);
	drawCylinder(360, 10, color, 0.85, 0.8, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 6, 0);
	drawCylinder(360, 10, color, 0.875, 0.85, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 8, 0);
	drawCylinder(360, 10, color, 0.85, 0.875, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 10, 0);
	drawCylinder(360, 10, color, 0.8, 0.85, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 12, 0);
	drawCylinder(360, 10, color, 0.6, 0.8, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.1, 1);
    glTranslated(0, 14, 0);
	drawCylinder(360, 10, color, 0.3, 0.6, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.05, 1);
    glTranslated(0, 30, 0);
	drawCylinder(360, 10, color, 0.3625, 0.3, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.05, 1);
    glTranslated(0, 32, 0);
	drawCylinder(360, 10, color, 0.4875, 0.3625, texture);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.05, 1);
    glTranslated(0, 34, 0);
	drawCylinder(360, 10, color, 0.7375, 0.4875, texture);
	glPopMatrix();

	glPopMatrix();
}

void drawTVFrame(float scale, float *color, int texture){
    // Save transformation
	glPushMatrix();
	// Offset, scale
	glScaled(scale, scale, scale);
	// Bottom frame
	glPushMatrix();
	glScaled(12, 0.5, 0.5);
    glTranslated(0, 0, 0);
	drawCube(color, true, true, texture);
	glPopMatrix();
	// Top frame
	glPushMatrix();
	glScaled(12, 0.5, 0.5);
    glTranslated(0, 12, 0);
	drawCube(color, true, true, texture);
	glPopMatrix();
	// Left frame
	glPushMatrix();
	glScaled(0.5, 6, 0.5);
    glTranslated(0, 0, 0);
	drawCube(color, true, true, texture);
	glPopMatrix();
	// Right frame
	glPushMatrix();
	glScaled(0.5, 6, 0.5);
    glTranslated(23, 0, 0);
	drawCube(color, true, true, texture);
	glPopMatrix();

	// Back screen
	glPushMatrix();
	glScaled(11.02, 5.52, -0.1);
    glTranslated(0.045, 0.09, 0);
	drawCube(color, true, true, texture);
	glPopMatrix();

	glPopMatrix();
}

void drawTVScreen(float scale, float *color, int texture){
    // Save transformation
	glPushMatrix();
	// Offset, scale
	glScaled(scale, scale, scale);
		// TV screen
		glPushMatrix();
		glScaled(11.02, 5.52, 0.4);
	    glTranslated(0.045, 0.09, 0);
		drawCube(color, true, true, texture);
		glPopMatrix();
	glPopMatrix();
}

//Draw Dums set
void drumCylinder(float *color){
	// Main cylinder
	glPushMatrix();
		drawCylinder(360, 10, color, 3, 3,-1);
		// Draw closing circles
		glPushMatrix();
			glTranslated(0, 0.75, 0);
			drawCircle(3, 360, 10,color, -1);
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, -0.75, 0);
			glRotated(180,1,0,0);
			drawCircle(3, 360, 10,color, -1);
		glPopMatrix();
	glPopMatrix();
}

void drumConnectorBase(float *color){
	glPushMatrix();
		glPushMatrix();
		drawCylinder(360, 10, color, 1, 2,-1);
		glPopMatrix();
	glPopMatrix();
}

void drumConnectorStick(float *color, int closed){
	glPushMatrix();
		drawCylinder(360, 10, color, 0.25, 0.25,-1);
		if (closed){
			glTranslated(0,1,0);
			drawCircle(0.25, 360, 10, color, -1);
		}
	glPopMatrix();
}

void drumLeg(float *color){
	glPushMatrix();
		drawCylinder(360, 10, color, 0.25, 0.25,-1);
	glPopMatrix();
}

void drumConnectorEndKnot(){
	float white[3] = {1.0, 1.0 ,1.0};
	glPushMatrix();
	glTranslated(0, 4, 0);
		// Top
		glPushMatrix();
			glScaled(1.5, 0.5, 1.5);
			glTranslated(0,-1,0);
			drawCylinderClosedEnds(360, 10, white , 1, 1, -1);
		glPopMatrix();
		//Middle
		glPushMatrix();
			glScaled(1,1,1);
			glTranslated(0,-2,0);
			drumConnectorStick(white, false);
		glPopMatrix();
		//Bottom
		glPushMatrix();
			glScaled(0.75, 0.5, 0.75);
			glTranslated(0,-7,0);
			drawCylinderClosedEnds(360, 10, white , 1, 1, -1);
		glPopMatrix();
	glPopMatrix();
}

void drawBase(float *color){
	glPushMatrix();
		glTranslated(0,3,0);
		glRotated(90, 1, 0, 0);
		glScaled(1, 1.5, 1);
		drumCylinder(color);
	glPopMatrix();
	// Drum connector on Top
	glPushMatrix();
		glTranslated(0, 6, 0);
		glScaled(0.30, 0.30, 0.30);
		drumConnectorBase(color);
	glPopMatrix();
	// Draw T connector on top
	glPushMatrix();
		glTranslated(0, 7, 0);
		drumConnectorStick(color, true);
	glPopMatrix();
	// Two support
	glPushMatrix();
		glRotated(60,0,1,0);
		glTranslated(-0.09, 7.75, -1.5);
		glScaled(0.18, 0.15, 1.5);
		drawCube(color, true, true, -1);
	glPopMatrix();
	glPushMatrix();
		glRotated(-60,0,1,0);
		glTranslated(-0.09, 7.75, -1.5);
		glScaled(0.18, 0.15, 1.5);
		drawCube(color, true, true, -1);
	glPopMatrix();
	// Two Connector sticks at the end of support
	glPushMatrix();
		glTranslated(-1.45, 8, -0.8);
		glScaled(0.75, 0.75, 0.75);
		drumConnectorStick(color, true);
	glPopMatrix();
	glPushMatrix();
		glTranslated(1.45, 8, -0.8);
		glScaled(0.75, 0.75, 0.75);
		drumConnectorStick(color, true);
	glPopMatrix();
	// Two connector ends
	glPushMatrix();
		glTranslated(-1.45, 8.75, -0.8);
		glScaled(0.15,0.15,0.15);
		drumConnectorEndKnot();
	glPopMatrix();
	glPushMatrix();
		glTranslated(1.45, 8.75, -0.8);
		glScaled(0.15,0.15,0.15);
		drumConnectorEndKnot();
	glPopMatrix();
	// Two connectors for TomTom
	glPushMatrix();
	glTranslated(0.25,0,-1.25);
	glRotated(45,0,1,0);
		glPushMatrix();
			glTranslated(-0.09 -1.45, 8.5, -1.5 -1);
			glScaled(0.18, 0.15, 1.5);
			drawCube(color, true, true, -1);
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.25,0,-1.25);
	glRotated(-45,0,1,0);
		glPushMatrix();
			glTranslated(0.09 + 1.45, 8.5, -1.5 -1);
			glScaled(0.18, 0.15, 1.5);
			drawCube(color, true, true, -1);
		glPopMatrix();
	glPopMatrix();
}

void drawTomTom(float *color){
	glPushMatrix();
		drumCylinder(color);
	glPopMatrix();
}

void drawTomTomStand(float *color){
	glPushMatrix();
	glTranslated(0, 15, 0);
		//Connector cube
		glPushMatrix();
			glScaled(2, 2, 4);
			glTranslated(-0.5, 0, -0.5);
			drawCube(color, true, true, -1);
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, -5, 0);
			glScaled(3,10,3);
			drumConnectorStick(color, true);
		glPopMatrix();
	glPopMatrix();
}

void triStand(float *color, int connector, int base){
	glPushMatrix();
		if (base){
			glTranslated(0,4,0);
		}else{
			glTranslated(0,5,0);
		}
		glScaled(1,3,1);
		drumConnectorStick(color, true);
	glPopMatrix();
	// Three connecting sticks
	// Left
	glPushMatrix();
		glTranslated(-1,1,1);
		glRotated(-45,0,1,0);
		glRotated(-45,1,0,0);
		glScaled(1,2,1);
		drumConnectorStick(color, true);
	glPopMatrix();
	//Right
	glPushMatrix();
		glTranslated(1,1,1);
		glRotated(45,0,1,0);
		glRotated(-45,1,0,0);
		glScaled(1,2,1);
		drumConnectorStick(color, true);
	glPopMatrix();
	// Back
	glPushMatrix();
		glTranslated(0,1,-1.5);
		glRotated(45,1,0,0);
		glScaled(1,2,1);
		drumConnectorStick(color, true);
	glPopMatrix();
	// Connnector Piece
	glPushMatrix();
		glTranslated(0,7,0);
		glScaled(0.15,0.15,0.15);
		drumCylinder(color);
	glPopMatrix();
	// connector end piece on the side of connector
	if (connector){
		glPushMatrix();
			glTranslated(0.45,7,0);
			glRotated(-90, 0, 0, 1);
			glScaled(0.15,0.15,0.15);
			drumConnectorEndKnot();
		glPopMatrix();
	}
}

void drawSnareDrum(float *color){
	// Draw triStand
	glPushMatrix();
		triStand(color,true,true);
		glPushMatrix();
			glTranslated(0,10,0);
			glRotated(180,1,0,0);
			triStand(color, false, false);
		glPopMatrix();
	glPopMatrix();
	// Draw Drum
	glPushMatrix();
		glTranslated(0,11,0);
		glScaled(1.5,1,1.5);
		drumCylinder(color);
	glPopMatrix();
}

void drawHiHat(float *color){
	// Draw triStand
	glPushMatrix();
		triStand(color,true,true);
		//Connector
		glPushMatrix();
			glTranslated(0,4 + 4,0);
			glScaled(1,2,1);
			drumConnectorStick(color, false);
		glPopMatrix();
		// Draw cymbal
		glPushMatrix();
			glTranslated(0,10,0);
			drawCircle(4, 360, 10, color, -1);
		glPopMatrix();
		// Connector piece
		glPushMatrix();
			glTranslated(0, 10, 0);
			glScaled(1.5,0.25,1.5);
			drumConnectorStick(color, true);
		glPopMatrix();
		// End Piece
		glPushMatrix();
			glTranslated(0, 11, 0);
			glScaled(0.5,1, 0.5);
			drumConnectorStick(color, true);
		glPopMatrix();
	glPopMatrix();
}

void drawRide(float *color){
	glPushMatrix();
		// triStand
		glPushMatrix();
			triStand(color,true,true);
		glPopMatrix();
	glPopMatrix();
	// Connector Rod
	glPushMatrix();
		glTranslated(0,2 + 7,0);
		glScaled(1,2,1);
		drumConnectorStick(color, false);
	glPopMatrix();
	// Sphere Connector
	glPushMatrix();
		glTranslated(0,11,0);
		drawSphere(10, 0.5, 0, 0, 0);
	glPopMatrix();
	// Connector rod from connector to cymbal
	glPushMatrix();
		glTranslated(1,12.5,-1);
		glRotated(-45,0,1,0);
		glRotated(-45,1,0,0);
		glScaled(0.5,5,0.5);
		drumConnectorStick(color, false);
	glPopMatrix();
	// Sphere connector at the end of the rod
	glPushMatrix();
		glTranslated(3.5, 16, -3.5);
		drawSphere(10, 0.25, 0, 0, 0);
	glPopMatrix();
	// Draw cymbal
	glPushMatrix();
		glTranslated(3.5,16, -3.5);
		glRotated(-45,0,1,0);
		glRotated(-15,1,0,0);
		drawCircle(4, 360, 10, color, -1);
	glPopMatrix();
}

void drawDrumsSet(float *color){
	//Draw Base
	glPushMatrix();
		drawBase(color);
	glPopMatrix();
	// Draw High Tom Tom (Right)
	glPushMatrix();
		glTranslated(3.5, 9, -2);
		glRotated(50, 0, 1, 0);
		glRotated(-45, 1, 0,0);
		glScaled(0.5, 1, 0.5);
		drawTomTom(color);
	glPopMatrix();
	// Draw Middle Tom Tom (Left)
	glPushMatrix();
		glTranslated(-3.5, 9, -2);
		glRotated(-50, 0, 1, 0);
		glRotated(-45, 1, 0,0);
		glScaled(0.5, 1, 0.5);
		drawTomTom(color);
	glPopMatrix();
	// Draw Floor Tom Tom (Left)
	glPushMatrix();
		glTranslated(-4.75, 2 + 3, -4.5);
		glScaled(0.75, 2, 0.75);
		drawTomTom(color);
	glPopMatrix();

	// Draw stands for Floor Tom Tom
	// Front
	glPushMatrix();
		glTranslated(-5, 0, -2);
		glScaled(0.15, 0.25, 0.15);
		drawTomTomStand(color);
	glPopMatrix();
	// Left
	glPushMatrix();
		glTranslated(-7.25, 0, -4.5);
		glScaled(0.15, 0.25, 0.15);
		glRotated(90,0,1,0);
		drawTomTomStand(color);
	glPopMatrix();
	// Back
	glPushMatrix();
		glTranslated(-5, 0, -7);
		glScaled(0.15, 0.25, 0.15);
		glRotated(180,0,1,0);
		drawTomTomStand(color);
	glPopMatrix();
	// Right
	glPushMatrix();
		glTranslated(-2.25, 0, -4.5);
		glScaled(0.15, 0.25, 0.15);
		glRotated(-90,0,1,0);
		drawTomTomStand(color);
	glPopMatrix();
	// Draw Snare Drum (Right)
	glPushMatrix();
		glTranslated(5,0.5,-4);
		glScaled(0.5,0.5,0.5);
		drawSnareDrum(color);
	glPopMatrix();
	// Draw HiHat Cymbal (Right behind Snare)
	glPushMatrix();
		glTranslated(7,0.5,-7);
		glScaled(0.75, 0.75, 0.75);
		drawHiHat(color);
	glPopMatrix();
	// Draw Ride Cymbal
	glPushMatrix();
		glTranslated(-7,0.5, 0);
		glScaled(0.75, 0.75, 0.75);
		drawRide(color);
	glPopMatrix();
}
