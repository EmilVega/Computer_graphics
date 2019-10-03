/****************************************************************************/
/* Prac. 3D Object3D from Primitive 2D                                        */
/****************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>

#define PI 3.14159265f
#define cup 1



// Global variables
int windowWidth  = 500;     // Initial Screen and viewport width
int windowHeight = 500;     // Initil Screen and viewport height

GLfloat zValue=200;
 GLfloat xRotated, yRotated, zRotated;

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

GLfloat PointsCurrent[9][3]={
  {0.0f, 40.0f, 0.0f},
  {55.0f,40.0f, 0.0f},
  {55.0f,50.0f, 0.0f},
  {15.0f,65.0f, 0.0f},
  {5.0f,115.0f, 0.0f},
  {15.0f,120.0f, 0.0f},
  {5.0f,125.0f, 0.0f},
  {15.0f, 135.0f, 0.0f},
{0.0f, 150.0f, 0.0f}};
GLfloat PointsNext[9][3]={
    {-10.0f, 40.0f, 0.0f},
    {55.0f,40.0f, 0.0f},
    {55.0f,50.0f, 0.0f},
    {15.0f,65.0f, 0.0f},
    {5.0f,115.0f, 0.0f},
    {15.0f,120.0f, 0.0f},
    {5.0f,125.0f, 0.0f},
    {15.0f, 135.0f, 0.0f},
  {0.0f, 150.0f, 0.0f}};


static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 2;

void DrawObj1(void)
{
  /*
  glBegin(GL_POLYGON);
    glColor3f(0.5f,0.5f,0.5f);
    glVertex3f( 0.0f, 100.0f, 0.0f);
    glVertex3f(50.0f, 100.0f, 0.0f);
    glVertex3f(50.0f,70.0f, 0.0f);
    glVertex3f( 0.0f,60.0f, 0.0f);
    glVertex3f( 0.0f,10.0f, 0.0f);
    glVertex3f( 50.0f,0.0f, 0.0f);
    glVertex3f( 0.0f,0.0f, 0.0f);
  glEnd();
  */
	int nSlices=1000;
	double dTheta=2.0*PI/nSlices;
	int nPoints=9;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*dTheta;
		double theta_next=(i+1)*dTheta;


    		//glVertex3f(x,y,z);

		glBegin(GL_POLYGON);
		for (int j=0; j<nPoints; j++)
		{
			double x=PointsCurrent[j][0];
			double y=PointsCurrent[j][1];
			double z=PointsCurrent[j][2];
    			//glVertex3f(Points[j][0]*cos(theta)+Points[j][2]*sin(theta),Points[j][1],-Points[j][0]*sin(theta)+Points[j][2]*cos(theta));
			glVertex3f(x,y,z);

			double xNext=PointsCurrent[j][0]*cos(theta_next)+PointsCurrent[j][2]*sin(theta_next);
 			double yNext=PointsCurrent[j][1];
			double zNext=-PointsCurrent[j][0]*sin(theta_next)+PointsCurrent[j][2]*cos(theta_next);
			//glVertex3f(xNext,yNext,zNext);

			PointsNext[j][0]=xNext;
			PointsNext[j][1]=yNext;
			PointsNext[j][2]=zNext;


		}
		glEnd();
                /*
		for (int j=1; j<nPoints; j++)
		{
		   glColor3f(0.5f,0.5f,0.5f);
		   glBegin(GL_LINE);


			glVertex3f(PointsNext[j][0],PointsNext[j][1],PointsNext[j][2]);
			glVertex3f(PointsNext[j-1][0],PointsNext[j-1][1],PointsNext[j-1][2]);
                   glEnd();
		   glBegin(GL_LINE);
			glVertex3f(PointsCurrent[j][0],PointsCurrent[j][1],PointsCurrent[j][2]);
			glVertex3f(PointsCurrent[j-1][0],PointsCurrent[j-1][1],PointsCurrent[j-1][2]);


		   glEnd();
		}
		*/
		for (int j=0; j<nPoints; j++)
		{
			PointsCurrent[j][0]=PointsNext[j][0];
			PointsCurrent[j][1]=PointsNext[j][1];
			PointsCurrent[j][2]=PointsNext[j][2];
		}
		//glVertex3f(x,y,z);
	}
	glFlush();
}


/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.4f,0.8f,0.5f,1.0f); //    Set background (clear) color to black
}


/* Callback handler for window re-paint event */
void display()
{

  GLfloat xL=-60;
  GLfloat yL=190;
  GLfloat zL=30;

  GLfloat light[3]={xL,yL,zL};
  GLfloat m[16];
   for (int i=0;i<16;i++){
     m[i]=0;
   }
   m[0]=m[5]=m[10]=1;
   m[7]=-1.0/yL;
   m[0]=-1;


   glClear (GL_COLOR_BUFFER_BIT); /*    clear screen */
   glMatrixMode(GL_MODELVIEW);    //    To operate on the model-view matrix
   glLoadIdentity();              //    Reset model-view matrix


   gluLookAt(0.0,-50.0,500.0,0.0,0.0,0.0, 0.0,1.0,0.0);      //   Define camera settings

   // glColor3f(0.0f, 0.0f, 1.0f);  //    Set the color of the object



   /* Draw a object */
   glRotatef(angleTheta[0],0.0,1.0,0.0);
   glBegin(GL_POLYGON);
    glColor3f(0,0,1);
    glVertex3f(-65,195,30);
    glVertex3f(-55,195,30);
    glVertex3f(-55,185,30);
    glVertex3f(-64,185,30);
   glEnd();



   glNewList(cup, GL_COMPILE);
      DrawObj1();
   glEndList();

   	glColor3f(1.0f,0.0f,0.0f);
    glCallList(cup);

  // glBegin(GL_POLYGON);
  //  glColor3f(1,0,0);
  //  glVertex3f(-50,0,0);
  //  glVertex3f(-10,0,0);
  //  glVertex3f(0,40,0);
  //  glVertex3f(-40,40,0);
  // glEnd();

   glPushMatrix();

   glTranslatef(xL,yL,zL);
   glMultMatrixf(m);
   glTranslatef(-xL,-yL,-zL);



   glColor3f(0.5,0.5,0.5);
   glCallList(cup);
   // DrawObj1();
   // glBegin(GL_POLYGON);
   //  glColor3f(0.5,0.5,0.5);
   //  glVertex3f(-50,0,0);
   //  glVertex3f(-10,0,0);
   //  glVertex3f(0,40,0);
   //  glVertex3f(-40,40,0);
   // glEnd();

   glPopMatrix();

   glFlush ();                   //   Render Object
}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspectRatio ratio of the new window
   GLfloat aspectRatio = (GLfloat)width / (GLfloat)height; //Compute aspect ratio


   glViewport(0, 0, width, height);  // Set the viewport to cover the new screen size

   // Set the aspectRatio ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  //    To operate on the Projection matrix
   glLoadIdentity();             //    Reset the projection matrix
   if (width >= height) {
      windowAreaXLeft   = -windowWidth * aspectRatio;
      windowAreaXRight  = windowWidth * aspectRatio;
      windowAreaYBottom = -windowHeight;
      windowAreaYTop    = windowHeight;
   } else {
      windowAreaXLeft   = -windowWidth;
      windowAreaXRight  = windowWidth;
      windowAreaYBottom = -windowHeight / aspectRatio;
      windowAreaYTop    = windowHeight / aspectRatio;
   }

   //Adjust vision cone
   //gluOrtho2D(windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop);

   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 2, 600.0); //    Define the shape of your viewing volume using glFrustum function



   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;
}

void mouse(int btn, int state, int x, int y)
{
 if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
       if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 1;
 if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 2;
 angleTheta[axis] += 2.0;
 if( angleTheta[axis] > 360.0 ) angleTheta[axis] -= 360.0;
 display();
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("Prac 4");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
