#include "utility.h"
#include "CSCIx229.h"

// Constants
const double len=1.5;  //  Length of axes

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
double z=0;     // Z variable
double w=1;     // W variable
double dim=3.0;   // Dimension of orthogonal box
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
int axes=1;       //  Display axes

float origin[3] = {0.0, 0.0, 0.0};
float red[3] = {1.0, 0.0, 0.0};
float green[3] = {0.0, 1.0, 0.0};
float blue[3] = {0.0, 0.0, 1.0};
float black[3] = {0.0, 0.0, 0.0};
float white[3] = {1.0, 1.0, 1.0};
float yellow[3] = {1.0, 1.0, 0.0};

// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   1.0;  // Elevation of light
float spotlight = 0.0;

void Project(double fov,double asp,double dim)
{
	 //  Tell OpenGL we want to manipulate the projection matrix
	 glMatrixMode(GL_PROJECTION);
	 //  Undo previous transformations
	 glLoadIdentity();
	 //  Perspective transformation
	 if (fov)
			gluPerspective(fov,asp,dim/16,16*dim);
	 //  Orthogonal transformation
	 else
			glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
	 //  Switch to manipulating the model matrix
	 glMatrixMode(GL_MODELVIEW);
	 //  Undo previous transformations
	 glLoadIdentity();
}

/*
 *  Display the scene
 */
void display()
{
	 //  Clear the image
	 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	 //  Enable Z-buffering in OpenGL
	 glEnable(GL_DEPTH_TEST);
	 //  Reset previous transforms
	 glLoadIdentity();
	 //  Perspective - set eye position
	 double Ex = -2*dim*Sin(th)*Cos(ph);
	 double Ey = +2*dim        *Sin(ph);
	 double Ez = +2*dim*Cos(th)*Cos(ph);
	 //  Erase the window and the depth buffer
	 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	 //  Enable Z-buffering in OpenGL
	 glEnable(GL_DEPTH_TEST);
	 //  Set perspective
	 glLoadIdentity();
	 gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);

	 //  Light switch
	 if (ylight)
	 {
			//  Translate intensity to color vectors
			float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
			float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
			float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
			//  Light direction
			float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
			//  Draw light position as ball (still no lighting here)
			glColor3f(1,1,1);
			drawSphere(10, 0.15, Position[0], Position[1], Position[2]);
			//  OpenGL should normalize normal vectors
			glEnable(GL_NORMALIZE);
			//  Enable lighting
			glEnable(GL_LIGHTING);
			//  glColor sets ambient and diffuse color materials
			glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);
			//  Enable light 0
			glEnable(GL_LIGHT0);
			//  Set ambient, diffuse, specular components and position of light 0
			glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
			glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
			glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
			glLightfv(GL_LIGHT0,GL_POSITION,Position);
	 }
	 if (spotlight){

			// specify and set the spotlight
			float spambient[] =    {0.2f,0.2f,0.2f,1.0f };
			float spdiffuse[] =    {1.0f,1.0f,1.0f,1.0f };
			float spspecular[] =    {1.0f,1.0f,1.0f,1.0f };

			float spposition[] =    {0.5f,5.0f,0.5f,1.0f};
			float spdirection[] = {0.0f,-1.0f,0.0f};
			float spotangle= 5.0;

			glEnable(GL_NORMALIZE);
			glEnable(GL_LIGHTING);

			glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHT1);

			glLightfv(GL_LIGHT1, GL_AMBIENT, spambient);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, spdiffuse);
			glLightfv(GL_LIGHT1, GL_SPECULAR, spspecular);
			glLightfv(GL_LIGHT1, GL_POSITION, spposition);
			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
			glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
			glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotangle);
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spdirection);

			glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);

		}

		if (!ylight && !spotlight){
			glDisable(GL_LIGHTING);
		}

		// Draw Scene Here:
	 	//drawTeaCup(0.1,blue);
		//drawRoundTable(1, green);
		// drawCube(green, false , true);
		// drawSquareTable(green);
		// drawChair(1, green);
    // drawLamp(green);
		glPushMatrix();
		drawSquareTable(green);
		glPopMatrix();
	 //  Draw axes
	 // No lighting from here
	 glDisable(GL_LIGHTING);
	 glColor3f(1,1,1);
	 if (axes)
	 {
			glBegin(GL_LINES);
			glVertex3d(0.0,0.0,0.0);
			glVertex3d(len,0.0,0.0);
			glVertex3d(0.0,0.0,0.0);
			glVertex3d(0.0,len,0.0);
			glVertex3d(0.0,0.0,0.0);
			glVertex3d(0.0,0.0,len);
			glEnd();
			//  Label axes
			glRasterPos3d(len,0.0,0.0);
			Print("X");
			glRasterPos3d(0.0,len,0.0);
			Print("Y");
			glRasterPos3d(0.0,0.0,len);
			Print("Z");
	 }
	 //  Display parameters
	 glWindowPos2i(5,5);
	 Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,"Perpective");
	 //  Flush and swap
	 glFlush();
	 glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
	 // Elapsed time in seconds
	 double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	 zh = fmod(90*t,360.0);
	 //  Tell GLUT it is necessary to redisplay the scene
	 glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	 //  Exit on ESC
	 if (ch == 27)
			exit(0);
	 //  Reset view angle
	 else if (ch == '0')
			th = ph = 0;
	 //  Toggle axes
	 else if (ch == 'a' || ch == 'A')
			axes = 1-axes;
	 //  Change field of view angle
	 else if (ch == '-' && ch>1)
			fov--;
	 else if (ch == '+' && ch<179)
			fov++;
	 //  Light elevation
	 else if (ch =='[')
			ylight -= 0.1;
	 else if (ch ==']')
			ylight += 0.1;

	 else if (ch == 'l'){
			if (ylight == 1.0) ylight = 0.0;
			else ylight = 1.0;
	 }

		else if (ch == 's'){
			if (spotlight == 1.0) spotlight = 0.0;
			else spotlight = 1.0;
		}

	 //  Reproject
	 //  Reproject
	 Project(45,asp,dim);
	 //  Tell GLUT it is necessary to redisplay the scene
	 glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
	 //  Right arrow key - increase azimuth by 5 degrees
	 if (key == GLUT_KEY_RIGHT) {
			th += 5;
		}
	 //  Left arrow key - decrease azimuth by 5 degrees
	 else if (key == GLUT_KEY_LEFT)
			th -= 5;
	 //  Up arrow key - increase elevation by 5 degrees
	 else if (key == GLUT_KEY_UP)
			ph += 5;
	 //  Down arrow key - decrease elevation by 5 degrees
	 else if (key == GLUT_KEY_DOWN)
			ph -= 5;
	 //  PageUp key - increase dim
	 else if (key == GLUT_KEY_PAGE_UP)
			dim += 0.1;
	 //  PageDown key - decrease dim
	 else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
			dim -= 0.1;
	 //  Keep angles to +/-360 degrees
	 th %= 360;
	 ph %= 360;
	 //  Tell GLUT it is necessary to redisplay the scene
	 glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
	 //  Ratio of the width to the height of the window
	 asp = (height>0) ? (double)width/height : 1;
	 //  Set the viewport to the entire window
	 glViewport(0,0, width,height);
	 //  Set projection
	 Project(45, asp, dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//  Initialize GLUT and process user parameters
	 glutInit(&argc,argv);
	 //  Request double buffered, true color window
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	 //  Request 500 x 500 pixel window
	 glutInitWindowSize(1000,1000);
	 //  Create the window
	 glutCreateWindow("S**t");
	 //  Tell GLUT to call "display" when the scene should be drawn
	 glutDisplayFunc(display);
	//  Tell GLUT to call "reshape" when the window is resized
	 glutReshapeFunc(reshape);
	 //  Tell GLUT to call "special" when an arrow key is pressed
	 glutSpecialFunc(special);
	 //  Tell GLUT to call "key" when a key is pressed
	 glutKeyboardFunc(key);
	 //  Pass control to GLUT so it can interact with the user
	 glutIdleFunc(idle);
	 glutMainLoop();
	 //  Return code
	 return 0;
}
