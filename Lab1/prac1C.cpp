
/****************************************************************************/
/* Prac. 1C Drawing and motion mouse          */
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

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat size = 3.0;   /* half side length of square */


void drawSquare(int x, int y)
{

	  y=wh-y;
	  glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
          //TODO: Draw a square centered at point x,y
	  glBegin(GL_POLYGON);
	 	   glVertex2f(x+size, y+size);
		    glVertex2f(x-size, y+size);
	 	   glVertex2f(x-size, y-size);
	    	glVertex2f(x+size, y-size);
	  glEnd();
	  glFlush();
}


/* rehaping routine called whenever window is resized
or moved */

void myReshape(GLsizei w, GLsizei h)
{

/* TODO: adjust clipping box// mapping window-viewport */

   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity(); 
   	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
   	

/* TODO: adjust viewport and clear */

	glViewport(0,0,w,h);
        glClearColor (0.0, 0.0, 0.0, 1.0);
   	glClear(GL_COLOR_BUFFER_BIT);
   	glFlush();

/* set global size for use by drawing routine */

	ww = w;
   	wh = h; 
}

void myinit(void)
{

   	glViewport(0,0,ww,wh);   

/* Pick 2D clipping window to match size of screen window 
This choice avoids having to scale object coordinates
each time window is resized */

   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity(); 
   	gluOrtho2D(0.0, (GLdouble) ww , 0.0, (GLdouble) wh );

/* set clear color to black and clear window */

   	glClearColor (0.0, 0.0, 0.0, 1.0);
	   glClear(GL_COLOR_BUFFER_BIT);
	   glFlush();



}

void mouse(int btn, int state, int x, int y)
{
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)   exit(0);   /*  TODO: Control the mouse action: exit when righ button is pressed  and state==GLUT_DOWN*/
}

/* display callback required by GLUT 3.0 */

void display(void)
{}

int main(int argc, char** argv)
{

   	glutInit(&argc,argv); // TODO: Initialize GLUT
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Prac 1C"); /*  TODO:  set window title */
   	myinit ();    /* set initial attributes */
   	glutReshapeFunc (myReshape); /* TODO myReshape callback invoked when window is reshaped */
   	glutMouseFunc (mouse);      /* TODO mouse callback invoked when mouse is used */
   	glutMotionFunc(drawSquare);   /* TODO drawSquare callback invoked with motion mouse */
	glutDisplayFunc(display);   /* TODO: display callback invoked when window opened */

   	glutMainLoop();     /*  TODO: Set the event-processing loop */

}
