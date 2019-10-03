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
 

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;
 
/* Initialize OpenGL Graphics */
void initGL() 
{
   glClearColor(0.0, 0.0, 0.0, 1.0); // TODO: Set background (clear) color to black
}

 
void drawSphere(float radio, int nlatitud , int nlongitud) // TODO: Create a draw sphere function
{
     float inct, incf;
     int i, j;
     float vertice[3];
     inct = 2 * PI / nlatitud;
     incf = PI / nlatitud;
     glBegin( GL_LINE_STRIP );
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
 
/* Callback handler for window re-paint event */
void display() 
{
      
   glClear (GL_COLOR_BUFFER_BIT); /* TODO: clear screen */
   glMatrixMode(GL_MODELVIEW);    // TODO: To operate on the model-view matrix
   glLoadIdentity();              // TODO: Reset model-view matrix
   
   
   gluLookAt(0.0,0.0,zValue,0.0,0.0,0.0, 0.0,1.0,0.0);      //TODO: Define camera settings
   
   glColor3f(0.0f, 0.0f, 1.0f);  // TODO: Set the color of the object
   /* Draw a sphere */
   drawSphere(50.0,10,10);        // TODO: Draw a Sphere, test using different primitives polygon/square/triangle/line STRIP/ what is the difference?, what does it happen with filled color?
    
   glFlush ();                   //TODO: Render Object
}
 
/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspectRatio ratio of the new window
   GLfloat aspectRatio = (GLfloat)width / (GLfloat)height; //Compute aspect ratio
 
   
   glViewport(0, 0, width, height);  // Set the viewport to cover the new screen size
 
   // Set the aspectRatio ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // TODO: To operate on the Projection matrix
   glLoadIdentity();             // TODO: Reset the projection matrix
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
   
   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 50.0, 250.0); // TODO: Define the shape of your viewing volume using glFrustum function
 
   
  
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
   glutCreateWindow("Prac 3B");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
