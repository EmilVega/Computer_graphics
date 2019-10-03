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
int windowWidth  = 500;     // Initial Screen and viewport width
int windowHeight = 500;     // Initil Screen and viewport height

GLfloat zValue=200;
 GLfloat xRotated, yRotated, zRotated;

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

GLfloat PointsCurrent[9][3]={{5.0f, 100.0f, 0.0f},
			{50.0f, 100.0f, 0.0f},
			{50.0f,90.0f, 0.0f},
			{50.0f,80.0f, 0.0f},
			{50.0f,70.0f, 0.0f},
			{5.0f,60.0f, 0.0f},
			{5.0f,10.0f, 0.0f},
			{50.0f,0.0f, 0.0f},
			{5.0f,0.0f, 0.0f}};
GLfloat PointsNext[9][3]={{5.0f, 100.0f, 0.0f},
			{50.0f, 100.0f, 0.0f},
			{50.0f,90.0f, 0.0f},
			{50.0f,80.0f, 0.0f},
			{50.0f,70.0f, 0.0f},
			{5.0f,60.0f, 0.0f},
			{5.0f,10.0f, 0.0f},
			{50.0f,0.0f, 0.0f},
			{5.0f,0.0f, 0.0f}};


GLfloat PointsRectCurrent[4][3]={{10.0f, 0.0f, 0.0f},
			{50.0f, 0.0f, 0.0f},
			{50.0f,90.0f, 0.0f},
			{10.0f,90.0f, 0.0f}};

GLfloat PointsRectNext[4][3]={{10.0f, 0.0f, 0.0f},
			{50.0f, 0.0f, 0.0f},
			{50.0f,90.0f, 0.0f},
			{10.0f,90.0f, 0.0f}};


GLfloat PointsSquareCurrent[4][3]={{10.0f, 0.0f, 0.0f},
			{50.0f, 0.0f, 0.0f},
			{50.0f,40.0f, 0.0f},
			{10.0f,40.0f, 0.0f}};

GLfloat PointsSquareNext[4][3]={{10.0f, 0.0f, 0.0f},
			{50.0f, 0.0f, 0.0f},
			{50.0f,40.0f, 0.0f},
			{10.0f,40.0f, 0.0f}};
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.0, 0.0, 0.0, 1.0); //    Set background (clear) color to black
}



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
	int nSlices=200;
	double dTheta=2.0*PI/nSlices;
	int nPoints=9;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*dTheta;
		double theta_next=(i+1)*dTheta;


    		//glVertex3f(x,y,z);
		glColor3f(0.0f,1.0f,0.0f);
		glBegin(GL_LINE_STRIP);
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

//Rotate Rectangle
void DrawObj2(void)
{
	int nSlices=10000;
	double dTheta=2.0*PI/nSlices;
	int nPoints=4;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*dTheta;
		double theta_next=(i+1)*dTheta;


    		//glVertex3f(x,y,z);
		glColor3f(0.0f,1.0f,0.0f);
		glBegin(GL_POLYGON);
		for (int j=0; j<nPoints; j++)
		{
			double x=PointsRectCurrent[j][0];
			double y=PointsRectCurrent[j][1];
			double z=PointsRectCurrent[j][2];

			glVertex3f(x,y,z);

			double xNext=x*cos(theta_next)+z*sin(theta_next);
 			double yNext=y;
			double zNext=-x*sin(theta_next)+z*cos(theta_next);
			//glVertex3f(xNext,yNext,zNext);

			PointsRectNext[j][0]=xNext;
			PointsRectNext[j][1]=yNext;
			PointsRectNext[j][2]=zNext;


		}
		glEnd();


		for (int j=0; j<nPoints; j++)
		{
			PointsRectCurrent[j][0]=PointsRectNext[j][0];
			PointsRectCurrent[j][1]=PointsRectNext[j][1];
			PointsRectCurrent[j][2]=PointsRectNext[j][2];
		}
		//glVertex3f(x,y,z);
	}
	glFlush();
}

//Sweep Square
void DrawObj3(void)
{
	int nSlices=20;
	double dx=PI*0.35;
	double dy=PI*0.35;
	double dz=PI*0.35;
	int nPoints=4;
	for (int i=0; i<nSlices;i++)
	{

    		//glVertex3f(x,y,z);
		glColor3f((char) rand()%256, (char) rand()%256, (char) rand()%256);

		glBegin(GL_QUADS);
		for (int j=0; j<nPoints; j++)
		{
			double x=PointsSquareCurrent[j][0];
			double y=PointsSquareCurrent[j][1];
			double z=PointsSquareCurrent[j][2];

			glVertex3f(x,y,z);

			double xNext=x+dx;
 			double yNext=y+dy;
			double zNext=z+dz;
			//glVertex3f(xNext,yNext,zNext);

			PointsSquareNext[j][0]=xNext;
			PointsSquareNext[j][1]=yNext;
			PointsSquareNext[j][2]=zNext;


		}
		glEnd();
		for (int j=1; j<nPoints; j++)
		{
		   glColor3f(0.5f,0.5f,0.5f);
		   glBegin(GL_POLYGON);


			glVertex3f(PointsSquareNext[j][0],PointsSquareNext[j][1],PointsSquareNext[j][2]);
			glVertex3f(PointsSquareCurrent[j][0],PointsSquareCurrent[j][1],PointsSquareCurrent[j][2]);
			glVertex3f(PointsSquareCurrent[j-1][0],PointsSquareCurrent[j-1][1],PointsSquareCurrent[j-1][2]);
			glVertex3f(PointsSquareNext[j-1][0],PointsSquareNext[j-1][1],PointsSquareNext[j-1][2]);


		   glEnd();
		}

		for (int j=0; j<nPoints; j++)
		{
			PointsSquareCurrent[j][0]=PointsSquareNext[j][0];
			PointsSquareCurrent[j][1]=PointsSquareNext[j][1];
			PointsSquareCurrent[j][2]=PointsSquareNext[j][2];
		}
		//glVertex3f(x,y,z);
	}


        //print matrix
	// glGetFloatv(GL_MODELVIEW_MATRIX,MVMatrix);
	// for (int i=0; i<16;i++);
	// {
	// 	print ("%f", MVMatrix[i]);
	// }
	glFlush();
}
/* Callback handler for window re-paint event */
void display()
{

   glClear (GL_COLOR_BUFFER_BIT); /*    clear screen */
   glMatrixMode(GL_MODELVIEW);    //    To operate on the model-view matrix
   glLoadIdentity();              //    Reset model-view matrix


   gluLookAt(0.0,0.0,200.0,0.0,0.0,0.0, 0.0,1.0,0.0);      //   Define camera settings

   glColor3f(0.0f, 0.0f, 1.0f);  //    Set the color of the object
   /* Draw a object */


   // DrawObj1();//Draw a cup from lines
   // DrawObj2();//Draw a cilinder from rectangle 2D
   DrawObj3();//Draw a parallelogram from a square 2D


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

   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 0.5, 200.0); //    Define the shape of your viewing volume using glFrustum function



   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;


}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("Prac 4C");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
