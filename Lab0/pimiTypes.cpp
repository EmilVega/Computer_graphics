/****************************************************************************/
/* This is a simple demo program written for CSE167  by Ravi Ramamoorthi    */
/* This program corresponds to the final OpenGL lecture on shading.         */
/* Modified September 2016 by Hoang Tran to exclusively use modern OpenGL   */
/*                                                                          */
/* This program draws some simple geometry, a plane with four pillars       */
/* textures the ground plane, and adds in a teapot that moves               */
/* Lighting effects are also included with fragment shaders                 */
/* The keyboard function should be clear about the keystrokes               */
/* The mouse can be used to zoom into and out of the scene                  */
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




void display(void)
{
	// clear all pixels

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 0.0f );
		glBegin(GL_POLYGON);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(-0.5f, 0.5f);
		glVertex2f( 0.5f, 0.5f);
		glVertex2f( 0.5f, -0.5f);
	glEnd();
	glFlush();
}




int main(int argc, char** argv)
{
	display();
	return 0;   /* ANSI C requires main to return int. */
}
