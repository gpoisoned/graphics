#include "utility.h"
#include "CSCIx229.h"

// Constants
const double len=10;  //  Length of axes

//  Globals
int th=-90;       // Azimuth of view angle
int ph=20;       // Elevation of view angle

double w=1;     // W variable
double dim=15.0;   // Dimension of orthogonal box
int fov=100;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
int axes=0;       //  Display axes

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
float shinyvec[1] = {1};    // Shininess (value)
int zh        =  90;  // Light azimuth

float ylight  =   4.0;  // Elevation of light

// Light Switch Flags
int light = 1;
int spotlight = 0;

int smooth = 1;
int local     =   0;  // Local Viewer Model

// Textures
int texture[20];
int noTexture = -1;

// Camera Parameters
// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx= 0.0f, lz= -1.0f;
// XZ position of the camera
float CameraX = 0.0f;
float CameraZ = 10.0f;
float fraction = 1.0f;

// Choose moving camera mode
int moveCamera = 0;

// SpotLight specific Params
float spambient[] =    {0.0f,0.0f,0.0f,1.0f };
float spdiffuse[] =    {1.0f,1.0f,1.0f,1.0f };

float normalSpecularLight[4] =  {0.05f,0.05f,0.05f,1.0f };
float yellowSpecularLight[4] = {1.0, 1.0, 0.0, 1.0};
float redSpecularLight[4]= {1.0, 0.0, 0.0, 1.0};
float blueSpecularLight[4]= {0.0, 0.0, 1.0, 1.0};
float greenSpecularLight[4]= {0.0, 1.0, 0.0, 1.0};

float whiteSpecularMaterial[3] = {1.0, 1.0, 1.0};
float redDiffuseMaterial[] = {1.0, 0.0, 0.0};
float greenEmissiveMaterial[] = {0.0, 1.0, 0.0};

float sp1_pos[4] = {0.0, 15.0, -15.0, 1.0};
float sp1_dir[4] = {0.0f, -1.0f, 0.0f, 0.0f};
float sp2_pos[4] = {0.0, 10.0, -5.0, 1.0};
float sp2_dir[4] = {0.0, 0.0, -1.0, 0.0};
float sp3_pos[4] = {0.0, 5.0, -5.0, 1.0};
float sp3_dir[4] = {0.0, 0.0, -1.0, 0.0};
float spotangle= 15.0;


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

void createSpotlight(int Light, float *spposition, float *spdirection,
										 float *spspecular, float spotangle, int showLoc){
	glShadeModel(GL_SMOOTH);
	//printf("******* Spotlight Turned on ********\n");
	// printf("x=%f y=%f z=%f\n", spposition[0], spposition[1], spposition[2]);
	// printf("dx=%f dy=%f dz=%f\n", spdirection[0], spdirection[1], spdirection[2]);

	if (showLoc){
		glColor3f(spspecular[0],spspecular[1],spspecular[2]);
		drawSphere(10, 0.15, spposition[0], spposition[1], spposition[2]);
	}

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	//  Location of viewer for specular calculations
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
	//  Two sided mode
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_COLOR_MATERIAL);
	//  Set specular colors
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);

	glEnable(Light);
	glLightfv(Light, GL_AMBIENT, spambient);
	glLightfv(Light, GL_DIFFUSE, spdiffuse);
	glLightfv(Light, GL_SPECULAR, spspecular);
	glLightfv(Light, GL_POSITION, spposition);

	glLightfv(Light, GL_SPOT_DIRECTION, spdirection);
	glLightf(Light, GL_SPOT_CUTOFF, spotangle);
	glLightf(Light, GL_SPOT_EXPONENT, 0.0);

	//  Set attenuation
	glLightf(Light,GL_CONSTANT_ATTENUATION ,1);
	glLightf(Light,GL_LINEAR_ATTENUATION   ,0);
	glLightf(Light,GL_QUADRATIC_ATTENUATION,0);

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
	 //  Erase the window and the depth buffer
	 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	 //  Enable Z-buffering in OpenGL
	 glEnable(GL_DEPTH_TEST);

	 glLoadIdentity();

	 if (moveCamera){
		//  printf("%f %f %f %f %f\n", x, z, lx, lz, angle);
		 gluLookAt(CameraX, 8.0, CameraZ , CameraX + lx  , 8.0, CameraZ + lz, 0, 1, 0);
	 }
	 else{
		double Ex = -2*dim*Sin(th)*Cos(ph);
		double Ey = +2*dim        *Sin(ph);
		double Ez = +2*dim*Cos(th)*Cos(ph);
		gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
	 }

	 //  Flat or smooth shading
	 glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

	 //  Light switch
	 if (light)
	 {
			//  Translate intensity to color vectors
			float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
			float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
			float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
			//  Light direction
			float Position[]  = {15*Cos(zh),ylight,15*Sin(zh),1};
			//  Draw light position as ball (still no lighting here)
			glColor3f(1,1,1);
			drawSphere(10, 0.15, Position[0], Position[1], Position[2]);
			//  OpenGL should normalize normal vectors
			glEnable(GL_NORMALIZE);
			//  Enable lighting
			glEnable(GL_LIGHTING);
			//  Location of viewer for specular calculations
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
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
	else{
			glDisable(GL_LIGHT0);
	}

	 if (spotlight){
			// Set the position of the spotlight
			// Overhead Light
			createSpotlight(GL_LIGHT1, sp1_pos, sp1_dir, normalSpecularLight, spotangle, 1);

			sp2_pos[0] = 3*Cos(zh);sp2_pos[2] = (3*Sin(zh)) -12;
			// RedSpotLight
			createSpotlight(GL_LIGHT2, sp2_pos, sp2_dir, redSpecularLight, 30, 1);

			sp3_pos[0] = 7*Cos(zh);sp3_pos[2] = (7*Sin(zh)) -12;
			// Green Light
			createSpotlight(GL_LIGHT3, sp3_pos, sp3_dir, greenSpecularLight, 20, 1);

			// Turn on both lamps
			float sp4_pos[4] = {-15.0, 15.0, -20.0, 1.0};
			float sp4_dir[4] = {0.0, -1.0, 0.0, 0.0};
			createSpotlight(GL_LIGHT4, sp4_pos, sp4_dir, normalSpecularLight, spotangle, 0);

			float sp5_pos[4] = {15.0, 15.0, -20.0, 1.0};
			float sp5_dir[4] = {0.0, -1.0, 0.0, 0.0};
			createSpotlight(GL_LIGHT5, sp5_pos, sp5_dir, normalSpecularLight, spotangle, 0);
		}else{
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
			glDisable(GL_LIGHT4);
			glDisable(GL_LIGHT5);
		}

		if (!light && !spotlight){
			glDisable(GL_LIGHTING);
		}

		// Draw Stuff

		glPushMatrix();
		glTranslated(0, 28, 0);
			drawRoom(texture[0], texture[1], 28);
		glPopMatrix();

		// TV and shelf
		glPushMatrix();
			glTranslated(-22, 0, 0);
			glRotated(90, 0, 1, 0);
			drawTVWithStand(-1,texture[8], texture[10]);
		glPopMatrix();

		// Draw Sofas
		glPushMatrix();
			glTranslated(-10, 0, 0);
			glRotated(-90, 0, 1, 0);
			drawSofas(white, texture[4]);
		glPopMatrix();

		//Dining Table
		glPushMatrix();
			glTranslated(15, 0, 0);
			drawDiningTable(white, texture[2], texture[7]);
		glPopMatrix();

		// Draw Shelf
		glPushMatrix();
			glTranslated(-15, 0, 22);
			drawShelf(3, white, texture[2]);
		glPopMatrix();

		// Draw Lamp
		// Left Lamp
		glPushMatrix();
			glTranslated(-15, 0, -20);
			glScaled(0.5, 0.5, 0.5);
			drawTableWithLampStyle1(white, texture[2], texture[10]);
		glPopMatrix();

		// Right Lamp
		glPushMatrix();
			glTranslated(15, 0, -20);
			glScaled(0.5, 0.5, 0.5);
			drawTableWithLampStyle2(white, texture[2], texture[10]);
		glPopMatrix();

		// Drums Set
		glPushMatrix();
			glTranslated(0, 0, -15);
			glScaled(0.75, 0.75, 0.75);
			drawDrumsSet(white, texture[9], texture[12], texture[10], texture[11]);
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
	 else if (ch == '0'){
		if(!moveCamera){
			th = -90;
			ph = 20;
		}else{
			CameraX = 0.0f;
			CameraZ = 8.0f;
			angle = 0.0;
			lx= 0.0;
			lz= -1.0;
		}
	 }

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
			if (light == 1) ylight = 0;
			else light = 1;
	 }

		else if (ch == 's'){
			if (spotlight == 1) {
				spotlight = 0;
				if (light == 0) light = 1;
			}
			else {
				spotlight = 1;
				if(light == 1) light = 0;
			}
		}

		else if (ch == 'm'){
			if (moveCamera == 0){
				moveCamera = 1;
			}
			else {
				moveCamera = 0;
			}
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
	 if (key == GLUT_KEY_RIGHT) {
		 if (moveCamera){
				angle += 0.04f;
				lx = sin(angle);
				lz = -cos(angle);
		  }
			else{
				th += 5;
			}
		}
	 else if (key == GLUT_KEY_LEFT){
		 if (moveCamera){
		 	angle -= 0.04f;
			lx = sin(angle);
			lz = -cos(angle);
		 }
		 else{
	 		 th -= 5;
		 }
	 }

	 else if (key == GLUT_KEY_UP){
		 if (moveCamera){
			 CameraX += lx * fraction;
			 CameraZ += lz * fraction;
		 }
		 else{
			ph += 5;
		 }
	 }

	 else if (key == GLUT_KEY_DOWN){
		if (moveCamera){
			CameraX -= lx * fraction;
			CameraZ -= lz * fraction;
		}
		else{
			ph -= 5;
		}
	 }

	 //  PageUp key - increase dim
	 else if (key == GLUT_KEY_PAGE_UP)
			dim += 1.0;
	 //  PageDown key - decrease dim
	 else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
			dim -=1.0;
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
	 glutCreateWindow("Jira3216");
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

	// Load Textures
	texture[0] = LoadTexBMP("wall.bmp");
	texture[1] = LoadTexBMP("floor.bmp");
	texture[2] = LoadTexBMP("furniture.bmp");
	texture[4] = LoadTexBMP("leather.bmp");
	texture[5] = LoadTexBMP("wood.bmp");
	texture[7] = LoadTexBMP("ceramic.bmp");
	texture[8] = LoadTexBMP("screen.bmp");

	// Drums specific
	texture[9] = LoadTexBMP("drums_cylinder.bmp");
	texture[10] = LoadTexBMP("metal.bmp");
	texture[11] = LoadTexBMP("gold.bmp");
	texture[12] = LoadTexBMP("black_brushed_metal.bmp");
	//  Pass control to GLUT so it can interact with the user
	ErrCheck("init");

	 glutMainLoop();
	 //  Return code
	 return 0;
}
