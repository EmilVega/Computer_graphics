/****************************************************************************/
/* Prac. 1A Geometric Primitive          */
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
#include <FreeImage.h>
#include <iomanip>

#define PI 3.14159265f

// Global variables
GLsizei wh = 500, ww = 500; /* initial window size */

GLfloat ballRadius=50.0f;

GLfloat windowAreaXLeft;
GLfloat windowAreaXRight;
GLfloat windowAreaYBottom;
GLfloat windowAreaYTop;

GLfloat XControl=0.0f;
GLfloat XIncr = 2.0f;
GLfloat YControl=0.0f;
GLfloat YIncr = 2.0f;

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // Ball's center (x, y) bounds

void initGL() {
   glClearColor(0.4f,0.8f,0.5f,1.0f); // Set background (clear) color to black
}

void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();   // Reset model-projetion matrix

  gluOrtho2D(windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop);

  glMatrixMode(GL_MODELVIEW);  // To operate on the model-view matrix
  glLoadIdentity();              // Reset model-view matrix

  if(XControl>=windowXMin && XControl<windowXMax && YControl==windowYMin){
    glTranslatef(XControl,0,0);
    XControl+=XIncr;
  }else if(XControl==windowXMax && YControl>=windowYMin && YControl<windowYMax){
    glTranslatef(XControl,YControl,0);
    YControl+=YIncr;
  } else if(XControl<=windowXMax && XControl>windowXMin && YControl==windowYMax){
    glTranslatef(XControl,YControl,0);
    XControl+=XIncr;
  } else if (XControl==windowXMin && YControl<=windowYMax && YControl>windowYMin) {
    glTranslatef(0,YControl,0);
    YControl+=YIncr;
  }

  //glScalef(XControl/250, XControl/250, 0.0f);  // Scale to (Sx, Sy)

   //Draw a circle
  glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f,0.0f,1.0f);
    glVertex2f(0.0f,0.0f);
    int numSegments=100;
    GLfloat angle;
    for (int i = 0; i <= numSegments; i++) {
      angle = i *2.0f *PI / numSegments;
      glVertex2f(cos(angle) * ballRadius, sin(angle)* ballRadius);
    }
  glEnd();
  glFlush();                                  // TODO: WRITE THE SENTENCE TO Render now


  if (XControl > windowXMax) {
     XControl = windowXMax;
     XIncr = -XIncr;
  } else if (XControl < windowXMin) {
     XControl = windowXMin;
     XIncr = -XIncr;
  }

  if (YControl > windowYMax) {
     YControl = windowYMax;
     YIncr = -YIncr;
  } else if (YControl < windowYMin) {
     YControl = windowYMin;
     YIncr = -YIncr;
  }
}


void myReshape(GLsizei w, GLsizei h)
{

  GLfloat factor= (GLfloat)w/(GLfloat)h;

  // Set the viewport to cover the new screen size
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();
  if (w >= h){
    windowAreaXLeft = (0.0-ballRadius) * factor;
    windowAreaXRight = (500.0+ballRadius) * factor;
    windowAreaYBottom = 0.0-ballRadius;
    windowAreaYTop = 500.0+ballRadius;
  } else {
    windowAreaXLeft = 0.0-ballRadius;
    windowAreaXRight = 500.0+ballRadius;
    windowAreaYBottom = (0.0-ballRadius) / factor;
    windowAreaYTop = (500.0+ballRadius) / factor;
  }

  //Adjust Window
  gluOrtho2D(windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop);

  //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
  windowXMin = windowAreaXLeft + ballRadius;
  windowXMax = windowAreaXRight - ballRadius;
  windowYMin = windowAreaYBottom + ballRadius;
  windowYMax = windowAreaYTop - ballRadius;
}

void timer (int value){
  glutPostRedisplay();
  glutTimerFunc (30,timer,0);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
  glutInit(&argc, argv);                 // TODO: Initialize GLUT
  glutInitWindowSize(ww,wh);
  glutCreateWindow("Practica 2A");                 // TODO: Create a window with the given title
  glutDisplayFunc(display);                             // TODO: Register display callback handler for window re-paint
  glutReshapeFunc(myReshape);
  glutTimerFunc(0, timer, 0);
  initGL();
  glutMainLoop();                            // TODO: Enter the event-processing loop
   return 0;
}
