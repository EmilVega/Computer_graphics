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



// Global variables
int windowWidth  = 700;     // Initial Screen and viewport width
int windowHeight = 700;     // Initil Screen and viewport height

GLfloat zValue=200;
 GLfloat xRotated, yRotated, zRotated;

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

GLfloat PointsCurrent[9][3]={
  {0.0f, 0.0f, 0.0f},
  {55.0f,0.0f, 0.0f},
  {55.0f,10.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {5.0f,75.0f, 0.0f},
  {15.0f,80.0f, 0.0f},
  {5.0f,85.0f, 0.0f},
  {15.0f, 95.0f, 0.0f},
{0.0f, 110.0f, 0.0f}};
GLfloat PointsNext[9][3]={
    {-10.0f, 0.0f, 0.0f},
    {55.0f,0.0f, 0.0f},
    {55.0f,10.0f, 0.0f},
    {15.0f,25.0f, 0.0f},
    {5.0f,75.0f, 0.0f},
    {15.0f,80.0f, 0.0f},
    {5.0f,85.0f, 0.0f},
    {15.0f, 95.0f, 0.0f},
  {0.0f, 110.0f, 0.0f}};

GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;


static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 2;

/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.4f,0.8f,0.5f,1.0f); //    Set background (clear) color to black
}



void DrawObj1(void)
{

	int nSlices=5000;
	double dTheta=1.0*PI/(nSlices*1000);
	int nPoints=9;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*dTheta;
		double theta_next=(i+0.5)*dTheta;


    		//glVertex3f(x,y,z);
		glColor3f(1.0f,0.0f,0.0f);
		// glBegin(GL_LINE_STRIP);
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


/* Callback handler for window re-paint event */
void display()
{

   glClear (GL_COLOR_BUFFER_BIT); /*    clear screen */
   glMatrixMode(GL_MODELVIEW);    //    To operate on the model-view matrix
   glLoadIdentity();              //    Reset model-view matrix


   gluLookAt(0.0,50.0,600.0,0.0,0.0,0.0,-1.0,1.0,0.0);      //   Define camera settings
   glRotatef(angleTheta[0],1.0,0.0,0.0);    //TODO: Rotate object in X
   glRotatef(angleTheta[1],0.0,1.0,0.0);
   glColor3f(1.0f, 0.0f, 0.0f);  //    Set the color of the objec
   /* Draw a object */


   DrawObj1();//Draw a cup from lines



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

   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 1, 700.0); //    Define the shape of your viewing volume using glFrustum function



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
   glutCreateWindow("QUIZ 1");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
