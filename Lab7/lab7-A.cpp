#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>

#define PI 3.14159265f

int windowWidth = 500;
int windowHeight = 500;
GLfloat zValue = 500;
GLfloat angleTheta[] = { 0.0,0.0,0.0 };
GLint axis = 2;
GLfloat windowXMax, windowXMin, windowYMax, windowYMin;
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

#define X 50
#define Z 80


int i, j, k, l, m;
const int nSegments = 20;

GLfloat Mpoints1[nSegments+1][nSegments+1][3];
GLfloat Mpoints2[nSegments+1][nSegments+1][3];
GLfloat vecT[4];
GLfloat u, v;
GLfloat P1[4][3], P2[4][3];
GLfloat ucoef[4], vcoef[4];

float Mcoef[4][4] = { {-1,3,-3,1}, {3,-6,3,0}, {-3,3,0,0}, {1,0,0,0} };
GLfloat ctrlpoints[4][4][3] = {
   {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0},
	{0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}},
   {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0},
	{0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}},
   {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0},
	{0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}},
   {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0},
	{0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};

void DrawAxis(void) {
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(500.0, 0.0, 0.0);
	glColor3f(0.3f, 0.0f, 0.0f);
	glVertex3f(-500.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);


	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0, 500.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glColor3f(0.0f, 0.3f, 0.0f);
	glVertex3f(0.0, -500.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);


	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0, 0.0, 500.0);
	glVertex3f(0.0, 0.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.3f);
	glVertex3f(0.0, 0.0, -500.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}

void fillVector(GLfloat vec[4], GLfloat M[4]) {
	for (int i = 0; i < 4; i++) {
		M[i] = 0;
		for (int j = 0; j < 4; j++)
			M[i] += vec[j] * Mcoef[j][i];
	}
}

void CalculateBezier() {
	for (i = 0; i <= nSegments; i++) {
		u = i / (float)nSegments;
		vecT[0] = u * u * u; vecT[1] = u * u; vecT[2] = u; vecT[3] = 1;
		fillVector(vecT, ucoef);
		glColor3f(1.0, 1.0, 1.0);
		for (j = 0; j <= nSegments; j++) {
			v = j / (float)nSegments;
			vecT[0] = v * v * v; vecT[1] = v * v; vecT[2] = v; vecT[3] = 1;
			fillVector(vecT, vcoef);
			for (k = 0; k < 4; k++)
				for (l = 0; l < 3; l++) {
					P1[k][l] = 0;
					P2[k][l] = 0;
					for (m = 0; m < 4; m++) {
						P1[k][l] += vcoef[m] * ctrlpoints[m][k][l];
						P2[k][l] += vcoef[m] * ctrlpoints[k][m][l];
					}
				}
			for (k = 0; k < 3; k++) {
				Mpoints1[i][j][k] = 0;
				Mpoints2[i][j][k] = 0;
				for (l = 0; l < 4; l++) {
					Mpoints1[i][j][k] += ucoef[l] * P1[l][k];
					Mpoints2[i][j][k] += ucoef[l] * P2[l][k];
				}
			}
		}
	}
}

void Draw(GLfloat M[nSegments + 1][nSegments + 1][3]) {
	CalculateBezier();
	for (i = 0; i <= nSegments; i++) {
		glBegin(GL_LINE_STRIP);
		for (j = 0; j <= nSegments; j++)
			glVertex3fv(M[i][j]);
		glEnd();
	}
}

void DrawBezier() {
	Draw(Mpoints1);
	Draw(Mpoints2);
	glFlush();
}


void initGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	DrawAxis();
	glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
	glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
	glScalef(150.0, 150.0, 150.0);
	DrawBezier();

}

void reshape(GLsizei width, GLsizei height) {
	GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
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
	windowXMin = windowAreaXLeft;
	windowXMax = windowAreaXRight;
	windowYMin = windowAreaYBottom;
	windowYMax = windowAreaYTop;

	glOrtho(windowXMin, windowXMax, windowYMin, windowYMax, -(windowXMax - windowXMin) / 2, (windowXMax - windowXMin) / 2);
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 2;
	angleTheta[axis] += 2.0;
	if (angleTheta[axis] > 360.0) angleTheta[axis] -= 360.0;
	display();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Prac 8");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	initGL();
	glutMainLoop();
	return 1;
}
