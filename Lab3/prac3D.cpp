/****************************************************************************/
/* Prac. 3C Working with the camera                                         */
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

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.0, 0.0, 0.0, 1.0); //   Set background (clear) color to black
}



void DrawCup(void)
{

  GLfloat vert [8][3]={
    {0,0,0},
    {50,0,0},
    {5,10,0},
    {5,50,0},
    {30,70,0},
    {45,80,0},
    {50,100,0},
    {0,100,0}
  };
  int i=0;

  glBegin(GL_POLYGON);
    for(i=0;i<8;i++){
      glVertex3fv(vert[i]);
    }
  glEnd();

glFlush();
}

/* Callback handler for window re-paint event */
void display()
{

   glClear (GL_COLOR_BUFFER_BIT); /*   clear screen */
   glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
   glLoadIdentity();              //   Reset model-view matrix


   gluLookAt(0.0,0.0,200.0,
               0.0,0.0,0.0,
               0.0,1.0,0.0);      //  Define camera settings

   glColor3f(1.0f, 0.0f, 0.0f);  //   Set the color of the object
   /* Draw a cube */
   DrawCup();        //   Draw a cube,

   glFlush ();                   //  Render Object
}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspectRatio ratio of the new window
   GLfloat aspectRatio = (GLfloat)width / (GLfloat)height; //Compute aspect ratio


   glViewport(0, 0, width, height);  // Set the viewport to cover the new screen size

   // Set the aspectRatio ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  //   To operate on the Projection matrix
   glLoadIdentity();             //   Reset the projection matrix
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

   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 0.5, 200.0); //   Define the shape of your viewing volume using glFrustum function



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
