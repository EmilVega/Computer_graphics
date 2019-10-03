/****************************************************************************/
/* Prac. 3A gluPerspective & glFrustum        */
/* use the Lookat function in the display callback to point                 */
/* the viewer                                                               */
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

GLfloat PointsCurrent[8][3]={
      {0.0f,-130.0f,0.0f},
      {50.0f,-130.0f,0.0f},
      {5.0f,-120.0f,0.0f},
      {5.0f,-80.0f,0.0f},
      {30.0f,-60.0f,0.0f},
      {45.0f,-50.0f,0.0f},
      {50.0f,-30.0f,0.0f},
      {0.0f,-30.0f,0.0f}};

GLfloat PointsNext[8][3]={
  {0.0f,-130.0f,0.0f},
  {50.0f,-130.0f,0.0f},
  {5.0f,-120.0f,0.0f},
  {5.0f,-80.0f,0.0f},
  {30.0f,-60.0f,0.0f},
  {45.0f,-50.0f,0.0f},
  {50.0f,-30.0f,0.0f},
  {0.0f,-30.0f,0.0f}};

GLfloat PointsSquareCurrent[4][3]={{0.0f, 30.0f, 0.0f},
			{40.0f, 30.0f, 0.0f},
			{40.0f,70.0f, 0.0f},
			{0.0f,70.0f, 0.0f}};

GLfloat PointsSquareNext[4][3]={{0.0f, 30.0f, 0.0f},
{40.0f, 30.0f, 0.0f},
{40.0f,70.0f, 0.0f},
{0.0f,70.0f, 0.0f}};


static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 2;

static GLfloat ChosenKey[] = {0.0,0.0};

static GLint sc = 1;


GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.4f,0.8f,0.5f,1.0f); // TODO: Set background (clear) color to black
}


void drawSphere(float radio, int nlatitud , int nlongitud) // TODO: Create a draw sphere function
{
     float inct, incf;
     int i, j;
     float vertice[3];
     inct = 2 * PI / nlatitud;
     incf = PI / nlatitud;
     // glBegin( GL_LINE_STRIP );
      glBegin(GL_POLYGON);
     for( i = 0; i < nlatitud; i++ )
     {

          vertice[0] = vertice[1] = 0;
          vertice[2] =- radio;
          glVertex3fv( vertice );
          for( j = 1; j < nlongitud-1; j++ )
          {
               vertice[0] = radio * cos(i*inct) * cos(j*incf-0.5*PI);
               vertice[1] = radio * sin(i*inct) * cos(j*incf-0.5*PI);
               vertice[2] = radio * sin(j*incf-0.5*PI);
               glVertex3fv( vertice );
               vertice[0] = radio*cos((i+1)*inct)*cos(j*incf-0.5*PI);
               vertice[1] = radio*sin((i+1)*inct)*cos(j*incf-0.5*PI);
               glVertex3fv(vertice);
          }
          vertice[0] = vertice[1] = 0;
          vertice[2] = radio;
          glVertex3fv( vertice );
     }
     glEnd();
}

//Sweep Square
void drawParallelogram(void)
{
	int nSlices=20;
	double dx=PI*0.35;
	double dy=PI*0.35;
	double dz=PI*0.35;
	int nPoints=4;
	for (int i=0; i<nSlices;i++)
	{


		// glColor3f((char) rand()%256, (char) rand()%256, (char) rand()%256);
    glColor3f(0.5f,0.5f,0.5f);

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

void drawCup(void)
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
	int nSlices=8000;
	double dTheta=2.0*PI/nSlices;
	int nPoints=9;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*dTheta;
		double theta_next=(i+1)*dTheta;


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

		for (int j=1; j<nPoints; j++)
		{
		   glColor3f(0.5f,0.5f,0.5f);
		   glBegin(GL_POLYGON);


			glVertex3f(PointsNext[j][0],PointsNext[j][1],PointsNext[j][2]);
			glVertex3f(PointsNext[j-1][0],PointsNext[j-1][1],PointsNext[j-1][2]);
                   glEnd();
		   glBegin(GL_LINE);
			glVertex3f(PointsCurrent[j][0],PointsCurrent[j][1],PointsCurrent[j][2]);
			glVertex3f(PointsCurrent[j-1][0],PointsCurrent[j-1][1],PointsCurrent[j-1][2]);


		   glEnd();
		}

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

  glClear(GL_COLOR_BUFFER_BIT);   /* TODO: clear screen */
  glMatrixMode(GL_MODELVIEW);    // TODO: To operate on the model-view matrix
  glLoadIdentity();  // TODO: Reset model-view matrix


  gluLookAt(0.0,0.0,230.0,0.0,0.0,0.0,0.0,1.0,0.0);     //TODO: Define camera settings
  drawParallelogram();


  glRotatef(angleTheta[0],1.0,0.0,0.0);    //TODO: Rotate object in X
  glRotatef(angleTheta[1],0.0,1.0,0.0);    //TODO: Rotate object in Y
  glPushMatrix();
  glTranslatef(ChosenKey[0], 0, 0);
  glTranslatef(ChosenKey[1], 0, 0);
  // glPushMatrix();

  glScalef(sc, sc, sc);

  glColor3f(1.0f,0.0f,0.0f);         // TODO: Set the color of the object
   /* Draw a sphere */
  drawSphere(20.0,50,50);         // TODO: Draw a Sphere, test using different primitives polygon/square/triangle/line STRIP/ what is the difference?, what does it happen with filled color?
  drawCup();

  glFlush();  //TODO: Render Object
}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspectRatio ratio of the new window
   GLfloat aspectRatio = (GLfloat)width / (GLfloat)height; //Compute aspect ratio


   glViewport(0, 0, width, height);  // Set the viewport to cover the new screen size

   // Set the aspectRatio ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);    // TODO: To operate on the Projection matrix
   glLoadIdentity();         // TODO: Reset the projection matrix
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


   gluPerspective(60.0,aspectRatio,50.0,350.0);    // TODO: Define the shape of your viewing volume using glFrustum function



   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;


}

void keyboard(unsigned char key, int x, int y)
{
switch (key) {
		case 'r':
			ChosenKey[0]+=3.0;
		break;
		case 'l':
			ChosenKey[1]-=3.0;
		break;
    case 's':
			sc+=1;
		break;
    case 'a':
			sc-=1;
		break;
		default:
		break;
	}
  display();
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
   glutCreateWindow("Prac 3B");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
