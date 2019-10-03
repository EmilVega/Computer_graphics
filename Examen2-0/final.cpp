/****************************************************************************/
/* Prac. 5A Polygonal mesh with triangles                                        */
/****************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <FreeImage.h>
#include <iomanip>
// Calling vertices and indices
#include "vertices.h"
#include "indices.h"

#define PI 3.14159265f
#define ObjTeapot 1

// Global variables
int windowWidth = 500;     // Initial Screen and viewport width 
int windowHeight = 500;     // Initil Screen and viewport height

GLfloat zValue = 8;
GLfloat xRotated, yRotated, zRotated;
GLfloat angleTheta[] = { 0.0,0.0,0.0 };
GLint axis = 2;

typedef GLfloat point[3];
point data[32][4][4];

typedef struct _LightInfo {
	GLfloat xyz[4];
	GLfloat *rgb;
	int enable;
} LightInfo;

typedef struct _LightBrightness {
	int num;
	GLfloat brightness;
} Light;

static GLfloat modelAmb[4] = { 0.1,0.1,0.1,1.0 };
static GLfloat matAmb[4] = { 0.9,0.9,0.9,1.0 };
static GLfloat matDiff[4] = { 0.8,0.8,0.8,1.0 };
static GLfloat matSpec[4] = { 0.9,0.9,0.9,1.0 };
static GLfloat matEmision[4] = { 0.0,0.0,0.0,1.0 };

GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat cian[] = { 0.0,1.0,1.0,1.0 };
GLfloat red[] = { 1.0,0.0,0.0,1.0 };
GLfloat dim[] = { 0.5,0.5,0.5,1.0 };

int lightState[8] = { 1,1,1,1,1,1,1,1 };
int allOff = 0;

int numActiveLights;
#define MAX_LIGHTS (sizeof(linfo)/sizeof(linfo[0]))
#define MIN_VALUE(a,b) (((a)<(b))?(a):(b))
#define MAX_VALUE(a,b) (((a)>(b))?(a):(b))

LightInfo linfo[] = {
	{{-2.5, 3.5, 3.0, 1.0}, green},
	{{0.0,  4.5, 2.0, 1.0}, cian},
	{{2.5,  3.5, 3.0, 1.0}, red},
};

void drawLight(LightInfo * info) {
	glPushMatrix();
	glTranslatef(info->xyz[0], info->xyz[1], info->xyz[2]);
	glColor3fv(info->rgb);
	glCallList(2);
	glPopMatrix();
}

void initLight(int num) {
	glLightf(GL_LIGHT0 + num, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0 + num, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0 + num, GL_QUADRATIC_ATTENUATION, 0.1);
	glLightfv(GL_LIGHT0 + num, GL_SPECULAR, dim);
}

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

#define X .525731112119133606 //TODO: Set the value of X
#define Z .850650808352039932 //TODO: Set the value of Z

static GLfloat vVertex[12][3] =
{
{-X,0.0,Z}, {X,0.0,Z}, {-X,0.0,-Z}, {X,0.0,-Z},
{0.0,Z,X}, {0.0,Z,-X}, {0.0,-Z,X}, {0.0,-Z,-X},
{Z,X,0.0}, {-Z,X,0.0}, {Z,-X,0.0}, {-Z,-X,0.0},
};

static GLuint tindices[20][3] =
{
{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
};

void DrawAxis(void) {
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(500 / 2, 0.0, 0.0);
	glColor3f(0.3f, 0.0f, 0.0f);
	glVertex3f(-500 / 2, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0, 500 / 2, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glColor3f(0.0f, 0.3f, 0.0f);
	glVertex3f(0.0, -500 / 2, 0.0);
	glVertex3f(0.0, 0.0, 0.0);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0, 0.0, 500 / 2);
	glVertex3f(0.0, 0.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.3f);
	glVertex3f(0.0, 0.0, -500 / 2);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}

void DrawTeapotObject()
{
	glPushMatrix();
	glTranslatef(0.0, 0.15, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	for (int k = 0; k < 32; k++) {
		glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
			0, 1, 12, 4, &data[k][0][0][0]);
		glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	}
	glPopMatrix();
}

void DrawBase(void) {
	GLfloat radius = 4.0f, h = 0.2f;
	int points = 100;
	GLfloat delta = 2 * PI / points;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, h, 0.0f);
	for (int i = 0; i <= points; i++) {
		GLfloat x = radius * cos(delta*i);
		GLfloat z = radius * sin(delta*i);
		glVertex3f(x, h, z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(radius, 0.0f, 0.0f);
	for (int i = 0; i <= points; i++) {
		GLfloat x = radius * cos(delta*i);
		GLfloat z = radius * sin(delta*i);
		glVertex3f(x, h, z);
		glVertex3f(x, 0.0f, z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i <= points; i++) {
		GLfloat x = radius * cos(delta*i);
		GLfloat z = radius * sin(delta*i);
		glVertex3f(x, 0.0f, z);
	}
	glEnd();
}

void DrawBase1(void)
{
	int r = 4, nlatitude = 100, nlongitude = 20;
	float varsliceslatitude = 2 * PI / nlatitude, varsliceslongitude = 0.05 / nlongitude;
	GLfloat vertex[3];
	for (int i = 0; i <= nlatitude; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= nlongitude; j++)
		{
			vertex[0] = r * cos((i + 1) * varsliceslatitude);
			vertex[2] = r * (j + 1) * varsliceslongitude;
			vertex[1] = r * sin((i + 1) * varsliceslatitude);
			glVertex3fv(vertex);
			vertex[0] = r * cos((i)*varsliceslatitude);
			vertex[1] = r * sin((i)*varsliceslatitude);
			glVertex3fv(vertex);
			vertex[0] = r * cos((i)*varsliceslongitude);
			vertex[1] = r * sin((i)*varsliceslongitude);
			glVertex3fv(vertex);
		}
		glEnd();
	}
}
/* Initialize OpenGL Graphics */
void initGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0); //   Set background (clear) color to black

	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

	numActiveLights = MIN_VALUE(MAX_LIGHTS, 8);
	for (int i = 0; i < numActiveLights; i++) {
		initLight(i);
	}

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, modelAmb);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmision);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);

	glNewList(ObjTeapot, GL_COMPILE);
	DrawTeapotObject();
	DrawBase();
	glEndList();

	glNewList(2, GL_COMPILE);
	glutSolidSphere(0.05, 20, 20);
	glEndList();
}

/* Callback handler for window re-paint event */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
	glLoadIdentity();

	gluLookAt(0.0, 5.0, zValue, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);      //  Define camera settings

	glPushMatrix();                                // TODO: Save State Matrix
	glDisable(GL_LIGHTING);
	for (int i = 0; i < MAX_LIGHTS; i++) {
		drawLight(&linfo[i]);
	}

	glPushMatrix();
	glEnable(GL_LIGHTING);
	for (int i = 0; i < numActiveLights; i++) {
		if (lightState[i]) {
			int num = i;
			// ld[i].num;
			//glScalef(1.0, 1.0, 1.0);

			glLightfv(GL_LIGHT0 + i, GL_POSITION, linfo[num].xyz);
			glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, linfo[num].rgb);
			glEnable(GL_LIGHT0 + i);
		}
		else {
			glDisable(GL_LIGHT0 + i);
		}
	}
	glPopMatrix();

	glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
	glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
	glRotatef(angleTheta[2], 0.0, 0.0, 1.0);

	glPushMatrix();
	glCallList(ObjTeapot);
	glPopMatrix();

	glPopMatrix();

	DrawAxis();

	glutSwapBuffers();
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
	gluPerspective(60.0, GLfloat(width) / GLfloat(height), 0.5, 500); //   Define the shape of your viewing volume using glFrustum function

	//Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
	windowXMin = windowAreaXLeft;
	windowXMax = windowAreaXRight;
	windowYMin = windowAreaYBottom;
	windowYMax = windowAreaYTop;
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 2;
	angleTheta[axis] += 2.0;
	if (angleTheta[axis] > 360.0) angleTheta[axis] -= 360.0;
	//if (angleTheta[axis] > 12.0) angleTheta[axis] -= 24.0;
	display();
}

void keyboard(unsigned char key, int x, int y)
{
	if (lightState[0] || lightState[1] || lightState[2])
		allOff = 0;
	else
		allOff = 1;

	switch (key)
	{
	case '1':
		lightState[0] = !lightState[0];
		break;

	case '2':
		lightState[1] = !lightState[1];
		break;

	case '3':
		lightState[2] = !lightState[2];
		break;

	case 'l':
		if (allOff) {
			lightState[0] = lightState[1] = lightState[2] = 1;
			allOff = 0;
		}
		else {
			lightState[0] = lightState[1] = lightState[2] = 0;
			allOff = 1;
		}

		break;

	default:
		break;
	}

	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	int i, j, k, m, n;
	for (i = 0; i < 32; i++) for (j = 0; j < 4; j++) for (k = 0; k < 4; k++) for (n = 0; n < 3; n++) {
		/* put teapot data into single array for subdivision */

		m = indices[i][j][k];
		for (n = 0; n < 3; n++) data[i][j][k][n] = vertices[m - 1][n];
	}
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitWindowSize(windowWidth, windowHeight);  // Initial win
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); /* TEXTURE */
	glutCreateWindow("Final Practice");      // Create window with given title
	glutDisplayFunc(display);     // Register callback handler for window re-paint
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	initGL();						// Our own OpenGL initialization   
	//loadTextures();
	glutMainLoop();               // Enter event-processing loop
	return 1;
}
