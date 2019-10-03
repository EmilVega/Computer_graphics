/****************************************************************************/
/* Prac.  9A      Curvas de Bezier                                          */
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
int windowWidth = 500;     // Initial Screen and viewport width 
int windowHeight = 500;     // Initil Screen and viewport height
float outNewCoordinates[3];
GLfloat P[4][3];

int cont = -1;
GLfloat Coef[4][4] = {
	{-1.0,3.0,-3.0,1.0},
	{3.0,-6.0,3.0,0.0},
	{-3.0,3.0,0.0,0.0},
	{1.0,0.0,0.0,0.0}
};
GLfloat zValue = 500;
GLfloat xRotated, yRotated, zRotated;
static GLfloat angleTheta[] = { 0.0,0.0,0.0 };
static GLint axis = 2;
int slices = 10;
GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

GLfloat ctrlpoints[4][4][3] = {
   {{-1.5, -1.5,  4.0}, {-0.5, -1.5,  2.0}, {0.5, -1.5, -1.0}, {1.5, -1.5,  2.0}},
   {{-1.5, -0.5,  1.0}, {-0.5, -0.5,  3.0}, {0.5, -0.5,  0.0}, {1.5, -0.5, -1.0}},
   {{-1.5,  0.5,  4.0}, {-0.5,  0.5,  0.0}, {0.5,  0.5,  3.0}, {1.5,  0.5,  4.0}},
   {{-1.5,  1.5, -2.0}, {-0.5,  1.5, -2.0}, {0.5,  1.5,  0.0}, {1.5,  1.5, -1.0}} };

void DrawAxis(void) {
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(250.0, 0.0, 0.0);
	glColor3f(0.3f, 0.0f, 0.0f);
	glVertex3f(-250.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);


	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0, 250.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glColor3f(0.0f, 0.3f, 0.0f);
	glVertex3f(0.0, -250.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);


	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0, 0.0, 250.0);
	glVertex3f(0.0, 0.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.3f);
	glVertex3f(0.0, 0.0, -250.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}

void Bezier() {
	int nSegments = 10;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= nSegments + 1; i++) {
		GLfloat t = (float)i / (float)nSegments;
		GLfloat Vect[4];
		Vect[0] = t * t*t;
		Vect[1] = t * t;
		Vect[2] = t;
		Vect[3] = 1;
		GLfloat tCoef[4] = { 0,0,0,0 };
		for (int j = 0; j < 4; j++) {
			GLfloat val = 0;
			for (int i = 0; i < 4; i++) {
				val = val + Vect[i] * Coef[i][j];
			}
			tCoef[j] = val;
		}
		GLfloat tCoefP[3] = { 0,0,0 };
		for (int j = 0; j < 3; j++) {
			GLfloat val = 0;
			for (int i = 0; i < 4; i++) {
				val = val + tCoef[i] * P[i][j];
			}
			tCoefP[j] = val;
		}
		glVertex3fv(tCoefP);
	}
	glEnd();
}

void BezierSurface(void) {
	int nSegments = 30;
	glColor3f(0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i <= nSegments + 1; i++) {
		glBegin(GL_POINTS);
		GLfloat u = (float)i / (float)nSegments;
		GLfloat u_vect[4] = { u*u*u, u*u, u, 1 };
		GLfloat u_coef[4] = { 0, 0, 0, 0 };

		for (int j = 0; j < 4; j++) {
			GLfloat val = 0;
			for (int i = 0; i < 4; i++) {
				val = val + u_vect[i] * Coef[i][j];
			}
			u_coef[j] = val;
		}

		for (int j = 0; j <= nSegments + 1; j++) {
			GLfloat v = (float)j / (float)nSegments;
			GLfloat v_vect[4] = { v*v*v, v*v, v, 1 };
			GLfloat v_coef[4] = { 0, 0, 0, 0 };

			for (int j = 0; j < 4; j++) {
				GLfloat val = 0;
				for (int i = 0; i < 4; i++) {
					val = val + v_vect[i] * Coef[i][j];
				}
				v_coef[j] = val;
			}

			GLfloat P[3] = { 0, 0, 0 };
			for (int m = 0; m < 3; m++) {
				GLfloat val = 0;
				for (int k = 0; k < 4; k++)
					for (int l = 0; l < 4; l++)
						val += u_coef[k] * v_coef[l]* ctrlpoints[k][l][m];
				P[m] = val;
			}
			glVertex3fv(P);
		}
		glEnd();
	}
}

/*void initGL1(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
		0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
}

void display1(void)
{
	int i, j;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	gluLookAt(0.0, 0.0, 1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);      //  Define camera settings

	glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
	glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
	glRotatef(angleTheta[2], 0.0, 0.0, 1.0);
	DrawAxis();

	glPushMatrix();
	//glRotatef(85.0, 1.0, 1.0, 1.0);
	glScalef(100.0f, 100.0f, 100.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (j = 0; j <= 8; j++) {
		glBegin(GL_LINE_STRIP);
		for (i = 0; i <= 30; i++)
			glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 8.0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (i = 0; i <= 30; i++)
			glEvalCoord2f((GLfloat)j / 8.0, (GLfloat)i / 30.0);
		glEnd();
	}
	glPopMatrix();
	glFlush();
}*/

/* Initialize OpenGL Graphics */
void initGL(void)
{
	glClearColor(0.75, 0.75, 0.75, 2.0); //   Set background (clear) color to black
	glEndList();
}

/* Callback handler for window re-paint event */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); /*   clear screen */
	glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
	glLoadIdentity();              //   Reset model-view matrix

	gluLookAt(0.0, 0.0, zValue, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);      //  Define camera settings

	/*glPushMatrix();
	//DrawBezierCurve();
	if (cont == 3)Bezier();
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	for (int i = 0; i <= cont; i++)
		glVertex3fv(P[i]);
	glEnd();

	glPopMatrix();*/
	/*glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			glBegin(GL_POINTS);
			glVertex3fv(ctrlpoints[i][j]);
			glEnd();
		}
	}*/

	glPushMatrix();
	glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
	glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
	glRotatef(angleTheta[2], 0.0, 0.0, 1.0);
	glPointSize(2.0f);
	glScalef(30.0f, 30.0f, 30.0f);
	BezierSurface();
	glPopMatrix();

	DrawAxis();
	glFlush();                   //  Render Object
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
		windowAreaXLeft = -windowWidth * aspectRatio;
		windowAreaXRight = windowWidth * aspectRatio;
		windowAreaYBottom = -windowHeight;
		windowAreaYTop = windowHeight;
	}
	else {
		windowAreaXLeft = -windowWidth;
		windowAreaXRight = windowWidth;
		windowAreaYBottom = -windowHeight / aspectRatio;
		windowAreaYTop = windowHeight / aspectRatio;
	}

	//Adjust vision cone
	//gluOrtho2D(windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop);
	//glOrtho(-250.0, 250.0, -250.0, 250.0, 0.0, 1000.0);
	gluPerspective(60.0, GLfloat(width) / GLfloat(height), 0.5, 1000.0); //   Define the shape of your viewing volume using glFrustum function
	//Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
	windowXMin = windowAreaXLeft;
	windowXMax = windowAreaXRight;
	windowYMin = windowAreaYBottom;
	windowYMax = windowAreaYTop;

	//glOrtho(windowXMin, windowXMax, windowYMin, windowYMax, -1 * (windowXMax - windowXMin) / 2, (windowXMax - windowXMin) / 2);
}

void GetMousePos3D(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y - 1;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	P[cont][0] = posX;
	P[cont][1] = posY;
	P[cont][2] = posZ;
	/* display(); */
}

void mouses(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (cont < 3)
			cont++;
		GetMousePos3D(x, y);
	}
	display();
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 2;
	angleTheta[axis] += 2.0;
	if (angleTheta[axis] > 360.0) angleTheta[axis] -= 360.0;

	display();
	//glutPostRedisplay();
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
	glutCreateWindow("Prac 8A");      // Create window with given title
	glutDisplayFunc(display);     // Register callback handler for window re-paint
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	initGL();                     // Our own OpenGL initialization
	glutMainLoop();               // Enter event-processing loop
	return 1;
}
