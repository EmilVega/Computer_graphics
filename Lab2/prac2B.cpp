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
#include <time.h>

void initGL() {
   glClearColor(0.4f,0.8f,0.5f,1.0f); // Set background (clear) color to black
}

void shearObj(float Shx, float Shy)
{
  float m[]={
     1.0,   Shy,  0.0, 0.0,
     Shx,  1.0,   0.0, 0.0,
     0.0,   0.0,    1.0, 0.0,
     0.0,   0.0,    0.0, 1.0};
 glMultMatrixf(m);
}

void display() {

   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer frame (background)
   glClearColor(0.4f,0.8f,0.5f,1.0f);                                     // TODO: Set background color to blue
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,500,0.0,500.0);                                    //TODO: Mapping window-viewport

   glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
   glLoadIdentity();              // Reset model-view matrix

  // Draw squares
  glTranslatef(100, 0, 0.0f);
  glBegin(GL_QUADS);				// TODO: Draw a square. Each set of 4 vertices form a quad
    glShadeModel(GL_SMOOTH);                 //TODO: DEFINE A SHADING TECHNIQUE (DEGRADE)
   	glColor3f(1.0f,0.0f,0.0f);              // TODO: SET VERTEX COLOR TO Red
   	glVertex2f(200.0f, 350.0f);             // DEFINE 1ST VERTEX x, y
    glColor3f(0.0f,0.0f,1.0f);              // TODO: SET VERTEX COLOR Blue
    glVertex2f(300.0f, 350.0f);             //TODO: DEFINE OTHER 3 VERTEX
    glVertex2f(300.0f, 450.0f);
    glVertex2f(200.0f, 450.0f);
   glEnd();

   glPushMatrix();


   shearObj(1.0,0.0);
   glTranslatef(-350, 0, 0.0f);
   glBegin(GL_QUADS);				// TODO: Draw a square. Each set of 4 vertices form a quad
    glShadeModel(GL_SMOOTH);                 //TODO: DEFINE A SHADING TECHNIQUE (DEGRADE)
   	glVertex2f(200.0f, 200.0f);             // DEFINE 1ST VERTEX x, y
    glColor3f(0.0f,0.0f,1.0f);              // TODO: SET VERTEX COLOR Blue
    glVertex2f(300.0f, 200.0f);             //TODO: DEFINE OTHER 3 VERTEX
    glVertex2f(300.0f, 300.0f);
    glVertex2f(200.0f, 300.0f);
   glEnd();

   glPopMatrix();

   glTranslatef(-200, 0, 0.0f);
   glBegin(GL_QUADS);				// TODO: Draw a square. Each set of 4 vertices form a quad
    glShadeModel(GL_SMOOTH);                 //TODO: DEFINE A SHADING TECHNIQUE (DEGRADE)
   	glColor3f(1.0f,0.0f,0.0f);              // TODO: SET VERTEX COLOR TO Red
   	glVertex2f(200.0f, 50.0f);             // DEFINE 1ST VERTEX x, y
    glColor3f(0.0f,0.0f,1.0f);              // TODO: SET VERTEX COLOR Blue
    glVertex2f(300.0f, 50.0f);             //TODO: DEFINE OTHER 3 VERTEX
    glVertex2f(300.0f, 150.0f);
    glVertex2f(200.0f, 150.0f);
   glEnd();

   glFlush();                                  // TODO: WRITE THE SENTENCE TO Render now
}



// void Timer(int value) {
//    glutPostRedisplay();    // Post a paint request to activate display()
//    glutTimerFunc(30, Timer, 0); // subsequent timer call at milliseconds
// }

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                            // TODO: Initialize GLUT
  glutInitWindowSize(500, 500);
  glutCreateWindow("Practica 2B");                 // TODO: Create a window with the given title
  glutInitWindowPosition(50, 50);
  glViewport(0,0,500,500);                             //TODO: SET THE VIEWPORT SIZE
  glutDisplayFunc(display);
  // glutTimerFunc(0, Timer, 0);                             // TODO: Register display callback handler for window re-paint
  initGL();
  glutMainLoop();                            // TODO: Enter the event-processing loop
   return 1;
}
