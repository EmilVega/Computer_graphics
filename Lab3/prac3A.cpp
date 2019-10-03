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



// Global variables
int windowWidth  = 500;     // Initial Screen and viewport width
int windowHeight = 500;     // Initil Screen and viewport height

GLfloat zValue=50;


GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 2;

static GLfloat ChosenKey[] = {0.0,0.0};

static GLint sc = 1;


// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

/* Initialize OpenGL Graphics */
void initGL()
{
  glClearColor(0.4f,0.8f,0.5f,1.0f);      // TODO: Set background (clear) color to black
}

/* Callback handler for window re-paint event */
void display()
{

  glClear(GL_COLOR_BUFFER_BIT);     /* TODO: clear screen */
  glMatrixMode(GL_MODELVIEW);       // TODO: To operate on the model-view matrix
  glLoadIdentity();                // TODO: Reset model-view matrix


  gluLookAt(0.0,0.0,230.0,0.0,0.0,0.0,0.0,1.0,0.0);     //TODO: Define camera settings
  glRotatef(angleTheta[0],1.0,0.0,0.0);    //TODO: Rotate object in X
  glRotatef(angleTheta[1],0.0,1.0,0.0);    //TODO: Rotate object in Y
  glTranslatef(ChosenKey[0], 0, 0);
  glTranslatef(ChosenKey[1], 0, 0);
  glScalef(sc, sc, sc);
  glColor3f(1.0f,0.0f,0.0f);   // TODO: Set the color lines of the object

   /* Draw a teapot */
  glutWireTeapot(100.0);

  glFlush();      //TODO: Render Object
}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspectRatio ratio of the new window
  GLfloat aspectRatio = (GLfloat) width/ (GLfloat)height;           //TODO: Compute aspect ratio
  GLfloat fovY=130.0;
  GLfloat zNear=50.0;
  GLfloat zFar=500.0;

  glViewport(0,0,width,height);     // TODO: Set the viewport to cover the new screen size

   // Set the aspectRatio ratio of the clipping area to match the viewport
  glMatrixMode(GL_PROJECTION);   // TODO: To operate on the Projection matrix
  glLoadIdentity();   // TODO: Reset the projection matrix
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


   // gluPerspective(fovY,aspectRatio,zNear,zFar);    /* TODO: FIRST=> Define a perspective projection with a fovy of 60 degrees and a near of 50.0 and far of 250.0 */

   // Transform to Frustum paramters

   const GLdouble pi = 3.1415926535897932384626433832795;
   GLdouble fW, fH;

   //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
   fH = tan( fovY / 360 * pi ) * zNear;
   fW = fH * aspectRatio;
   glFrustum( -fW, fW, -fH, fH, zNear, zFar );

   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;

   printf("%f \n",windowYMax);
                                // TODO: SECOND=> Define the shape of your viewing volume using glFrustum function

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
   glutCreateWindow("Prac 3A");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);         // Register callback handler for mouse movements
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
