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

void display() {

   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer frame (background)
   glClearColor(0.4f,0.8f,0.5f,1.0f);                                     // TODO: Set background color to blue
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,500,0.0,500.0);                                    //TODO: Mapping window-viewport

   // Draw a CHOOSE COLOR 1x1 Square centered at window

   glBegin(GL_QUADS);				// TODO: Draw a square. Each set of 4 vertices form a quad
    glShadeModel(GL_SMOOTH);                 //TODO: DEFINE A SHADING TECHNIQUE (DEGRADE)
   	glColor3f(1.0f,0.0f,0.0f);              // TODO: SET VERTEX COLOR TO Red
   	glVertex2f(20.0f, 200.0f);             // DEFINE 1ST VERTEX x, y
    glColor3f(0.0f,0.0f,1.0f);              // TODO: SET VERTEX COLOR Blue
    glVertex2f(120.0f, 200.0f);             //TODO: DEFINE OTHER 3 VERTEX
    glVertex2f(120.0f, 300.0f);
    glVertex2f(20.0f, 300.0f);
   glEnd();

   glBegin(GL_QUADS);				// TODO: Draw a square. Each set of 4 vertices form a quad
    glShadeModel(GL_SMOOTH);                 //TODO: DEFINE A SHADING TECHNIQUE (DEGRADE)
   	glVertex2f(140.0f, 200.0f);             // DEFINE 1ST VERTEX x, y
    glColor3f(0.0f,0.0f,1.0f);              // TODO: SET VERTEX COLOR Blue
    glVertex2f(240.0f, 200.0f);             //TODO: DEFINE OTHER 3 VERTEX
    glVertex2f(240.0f, 300.0f);
    glVertex2f(140.0f, 300.0f);
   glEnd();

   glBegin(GL_QUADS);				// TODO: Draw a square. Each set of 4 vertices form a quad
    glShadeModel(GL_SMOOTH);                 //TODO: DEFINE A SHADING TECHNIQUE (DEGRADE)
   	glColor3f(1.0f,0.0f,0.0f);              // TODO: SET VERTEX COLOR TO Red
   	glVertex2f(260.0f, 200.0f);             // DEFINE 1ST VERTEX x, y
    glColor3f(0.0f,0.0f,1.0f);              // TODO: SET VERTEX COLOR Blue
    glVertex2f(360.0f, 200.0f);             //TODO: DEFINE OTHER 3 VERTEX
    glVertex2f(360.0f, 300.0f);
    glVertex2f(260.0f, 300.0f);
   glEnd();

   glBegin(GL_QUADS);				// TODO: Draw a square. Each set of 4 vertices form a quad
    glShadeModel(GL_SMOOTH);                 //TODO: DEFINE A SHADING TECHNIQUE (DEGRADE)
   	glColor3f(1.0f,0.0f,0.0f);              // TODO: SET VERTEX COLOR TO Red
   	glVertex2f(380.0f, 200.0f);             // DEFINE 1ST VERTEX x, y
    glVertex2f(480.0f, 200.0f);             //TODO: DEFINE OTHER 3 VERTEX
    glVertex2f(480.0f, 300.0f);
    glVertex2f(380.0f, 300.0f);
   glEnd();

   glFlush();                                  // TODO: WRITE THE SENTENCE TO Render now
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                            // TODO: Initialize GLUT
   	glutCreateWindow("Practical 1A");                 // TODO: Create a window with the given title
        glutInitWindowSize(500, 500);                   // TODO: Set the window's initial width & height
        glViewport(0,0,500,500);                             //TODO: SET THE VIEWPORT SIZE
        glutDisplayFunc(display);                             // TODO: Register display callback handler for window re-paint
        glutMainLoop();                            // TODO: Enter the event-processing loop
   return 0;
}
