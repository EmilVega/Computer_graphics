
/****************************************************************************/
/* Prac. 1B Geometric Primitive          */
/* Two-Dimensional Sierpinski Gasket          */
/* Generated Using Randomly Selected Vertices */
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

void myinit()
{
 
/* attributes */

      glClearColor(1.0, 1.0, 1.0, 1.0); /* TODO: Set white background */
      glColor3f(0.0, 1.0, 0.0); /* TODO: set color draw in blue */

/* set up viewing */
/* 500 x 500 window with origin lower left */

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0.0, 50.0, 0.0, 50.0); // TODO: Mapping WINDOW-VIEWPORT
}

void display( void )
{

     

    GLfloat vertices[3][2]={{0.0,0.0},{25.0,50.0},{50.0,0.0}}; /* TODO: set the vertixes of the first/external triangle */

    int j, k;
    int rand();       /* standard random number generator */
    GLfloat p[2] ={7.5,5.0};  /* An arbitrary initial point inside traingle */

    glClear(GL_COLOR_BUFFER_BIT);  /*TODO: clear the window */


/* compute and plots 50000 new points */

	glBegin(GL_POINTS);  //TODO: set the POINT primitive type 

    for( k=0; k<50000; k++)
    {
         j=rand()%3; /* pick a vertex at random */


     /* TODO: Compute point halfway between selected vertex and old point */

         p[0] = (p[0]+vertices[j][0])/2.0; 
         p[1] = (p[1]+vertices[j][1])/2.0;
   
     /* TODO: plot new point */

        glVertex2fv(p); 
   
     }
	 glEnd();
     glFlush(); /* TODO: clear buffers, and render now */
 }

int main(int argc, char** argv)
{

/* Standard GLUT initialization */

    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); /* default, not needed */
    glutInitWindowSize(500,500); /* TODO: Set the window size 500 x 500 pixel */
    glutInitWindowPosition(0,0); /* place window top left on display */
    glutCreateWindow("Practical 1b Sierpinski Gasket"); /* TODO: Set window title */
    glutDisplayFunc(display); /* TODO: Register display callback handler for window re-paint, display callback invoked when window opened */

    myinit(); /* set attributes */

    glutMainLoop(); // TODO: Enter the event-processing loop
    return 0;
}

