/****************************************************************************/
/* Project 1: The project is focused on draw 6 unique and diferent pieces
of chess and apply them several transformations (Topics: primitives, matrices,
transformations, push and pop matrixes, instances, camera transformations,
callbacks )

Students: Maria Molina-Ron & Emil Vega-Gualan
Course: Noveno TICs                                                         */
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
#include <math.h>
#include <iostream>

#define PI 3.14159265f

// variables to the lists
#define queen 1
#define king 2
#define bishop 3
#define knight 4
#define pawn 5
#define rook 6
#define flr 7

//Variables to make de transformations to the pieces
float kinc=0;
float rZx=0;
float sinc=1;
float qinc=0;
float qinc2=0;
float move_z=0.0;
float dec=1;
float inc_z=0.0;

// Global variables
int windowWidth  = 700;     // Initial Screen and viewport width
int windowHeight = 700;     // Initil Screen and viewport height

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;


static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 0;

GLfloat PointsQueen[27][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {10.0f, 40.0f, 0.0f},
  {8.0f, 50.0f, 0.0f},
  {7.5f, 67.5f, 0.0f},
  {18.0f, 70.0f, 0.0f},
  {22.0f, 75.0f, 0.0f},
  {18.0f, 74.0f, 0.0f},
  {18.0f, 76.0f, 0.0f},
  {12.0f, 76.0f, 0.0f},
  {12.0f, 80.0f, 0.0f},
  {17.0f, 82.5f, 0.0f},
  {13.0f, 85.0f, 0.0f},
  {15.0f, 95.0f, 0.0f},
  {20.0f, 103.0f, 0.0f},
  {17.5f, 105.0f, 0.0f},
  {15.0f, 100.0f, 0.0f},
  {10.0f, 102.5f, 0.0f},
  {5.0f, 100.0f, 0.0f},
  {5.0f, 105.0f, 0.0f},
  {0.0f, 115.0f, 0.0f}};

// Matrixes to draw the pieces

GLfloat PointsQueen2[27][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {10.0f, 40.0f, 0.0f},
  {8.0f, 50.0f, 0.0f},
  {7.5f, 67.5f, 0.0f},
  {18.0f, 70.0f, 0.0f},
  {22.0f, 75.0f, 0.0f},
  {18.0f, 74.0f, 0.0f},
  {18.0f, 76.0f, 0.0f},
  {12.0f, 76.0f, 0.0f},
  {12.0f, 80.0f, 0.0f},
  {17.0f, 82.5f, 0.0f},
  {13.0f, 85.0f, 0.0f},
  {15.0f, 95.0f, 0.0f},
  {20.0f, 103.0f, 0.0f},
  {17.5f, 105.0f, 0.0f},
  {15.0f, 100.0f, 0.0f},
  {10.0f, 102.5f, 0.0f},
  {5.0f, 100.0f, 0.0f},
  {5.0f, 105.0f, 0.0f},
  {0.0f, 115.0f, 0.0f}};

GLfloat PointsKing[31][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {10.0f, 40.0f, 0.0f},
  {8.0f, 50.0f, 0.0f},
  {7.5f, 67.5f, 0.0f},
  {19.0f, 70.0f, 0.0f},
  {20.0f, 71.0f, 0.0f},
  {19.0f, 72.5f, 0.0f},
  {16.0f, 72.5f, 0.0f},
  {16.0f, 75.0f, 0.0f},
  {12.0f, 75.0f, 0.0f},
  {12.0f, 80.0f, 0.0f},
  {15.0f, 82.5f, 0.0f},
  {14.0f, 85.0f, 0.0f},
  {20.0f, 100.0f, 0.0f},
  {10.0f, 100.0f, 0.0f},
  {10.0f, 102.0f, 0.0f},
  {7.5f, 102.0f, 0.0f},
  {10.0f, 104.0f, 0.0f},
  {7.5f, 106.0f, 0.0f},
  {10.0f, 106.0f, 0.0f},
  {10.0f, 112.0f, 0.0f},
  {4.0f, 112.0f, 0.0f},
  {6.0f, 117.0f, 0.0f},
  {0.0f, 120.0f, 0.0f}};

GLfloat PointsKing2[31][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {10.0f, 40.0f, 0.0f},
  {8.0f, 50.0f, 0.0f},
  {7.5f, 67.5f, 0.0f},
  {19.0f, 70.0f, 0.0f},
  {20.0f, 71.0f, 0.0f},
  {19.0f, 72.5f, 0.0f},
  {16.0f, 72.5f, 0.0f},
  {16.0f, 75.0f, 0.0f},
  {12.0f, 75.0f, 0.0f},
  {12.0f, 80.0f, 0.0f},
  {15.0f, 82.5f, 0.0f},
  {14.0f, 85.0f, 0.0f},
  {20.0f, 100.0f, 0.0f},
  {10.0f, 100.0f, 0.0f},
  {10.0f, 102.0f, 0.0f},
  {7.5f, 102.0f, 0.0f},
  {10.0f, 104.0f, 0.0f},
  {7.5f, 106.0f, 0.0f},
  {10.0f, 106.0f, 0.0f},
  {10.0f, 112.0f, 0.0f},
  {4.0f, 112.0f, 0.0f},
  {6.0f, 117.0f, 0.0f},
  {0.0f, 120.0f, 0.0f}};

GLfloat PointsRook[19][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {11.0f, 40.0f, 0.0f},
  {9.0f, 70.0f, 0.0f},
  {18.0f, 70.0f, 0.0f},
  {18.0f, 75.0f, 0.0f},
  {23.0f, 75.0f, 0.0f},
  {23.0f, 90.0f, 0.0f},
  {12.0f, 90.0f, 0.0f},
  {12.0f, 82.0f, 0.0f},
  {9.0f, 82.0f, 0.0f},
  {9.0f, 90.0f, 0.0f},
  {0.0f, 90.0f, 0.0f}};

GLfloat PointsRook2[19][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {11.0f, 40.0f, 0.0f},
  {9.0f, 70.0f, 0.0f},
  {18.0f, 70.0f, 0.0f},
  {18.0f, 75.0f, 0.0f},
  {23.0f, 75.0f, 0.0f},
  {23.0f, 90.0f, 0.0f},
  {12.0f, 90.0f, 0.0f},
  {12.0f, 82.0f, 0.0f},
  {9.0f, 82.0f, 0.0f},
  {9.0f, 90.0f, 0.0f},
  {0.0f, 90.0f, 0.0f}};

GLfloat Pawn1[23][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {10.0f, 40.0f, 0.0f},
  {8.0f, 50.0f, 0.0f},
  {8.0f, 55.5f, 0.0f},
  {18.0f, 55.0f, 0.0f},
  {22.0f, 59.0f, 0.0f},
  {18.0f,64.0f, 0.0f},
  {8.0f, 64.0f, 0.0f},
  {11.0f, 67.0f, 0.0f},
  {15.0f, 70.0f, 0.0f},
  {16.0f, 77.0f, 0.0f},
  {15.0f,85.0f, 0.0f},
  {10.0f, 91.0f, 0.0f},
  {5.0f, 92.0f, 0.0f},
  {4.0f, 92.0f, 0.0f},
  {2.0f, 93.0f, 0.0f}};

GLfloat Pawn2[23][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {10.0f, 40.0f, 0.0f},
  {8.0f, 50.0f, 0.0f},
  {8.0f, 55.5f, 0.0f},
  {18.0f, 55.0f, 0.0f},
  {22.0f, 59.0f, 0.0f},
  {18.0f,64.0f, 0.0f},
  {8.0f, 64.0f, 0.0f},
  {11.0f, 67.0f, 0.0f},
  {15.0f, 70.0f, 0.0f},
  {16.0f, 77.0f, 0.0f},
  {15.0f,85.0f, 0.0f},
  {10.0f, 91.0f, 0.0f},
  {5.0f, 92.0f, 0.0f},
  {4.0f, 93.0f, 0.0f},
  {2.0f, 93.0f, 0.0f}};

GLfloat Bishop1[29][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {10.0f, 40.0f, 0.0f},
  {8.0f, 50.0f, 0.0f},
  {8.0f, 57.0f, 0.0f},
  {19.0f, 60.0f, 0.0f},
  {20.0f, 61.0f, 0.0f},
  {19.0f, 62.5f, 0.0f},
  {16.0f, 62.5f, 0.0f},
  {16.0f, 65.0f, 0.0f},
  {12.0f, 65.0f, 0.0f},
  {12.0f, 70.0f, 0.0f},
  {15.0f, 72.0f, 0.0f},
  {14.0f, 75.0f, 0.0f},
  {15.0f, 80.0f, 0.0f},
  {16.0f, 85.0f, 0.0f},
  {15.0f, 90.0f, 0.0f},
  {13.0f, 95.0f, 0.0f},
  {10.0f, 98.5f, 0.0f},
  {7.0f, 102.0f, 0.0f},
  {5.0f, 106.0f, 0.0f},
  {6.0f, 108.0f, 0.0f},
  {5.0f, 110.0f, 0.0f}};

GLfloat Bishop2[29][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {19.0f, 30.0f, 0.0f},
  {10.0f, 40.0f, 0.0f},
  {8.0f, 50.0f, 0.0f},
  {8.0f, 57.0f, 0.0f},
  {19.0f, 60.0f, 0.0f},
  {20.0f, 61.0f, 0.0f},
  {19.0f, 62.5f, 0.0f},
  {16.0f, 62.5f, 0.0f},
  {16.0f, 65.0f, 0.0f},
  {12.0f, 65.0f, 0.0f},
  {12.0f, 70.0f, 0.0f},
  {15.0f, 72.0f, 0.0f},
  {14.0f, 75.0f, 0.0f},
  {15.0f, 80.0f, 0.0f},
  {16.0f, 85.0f, 0.0f},
  {15.0f, 90.0f, 0.0f},
  {13.0f, 95.0f, 0.0f},
  {10.0f, 98.5f, 0.0f},
  {7.0f, 102.0f, 0.0f},
  {5.0f, 106.0f, 0.0f},
  {6.0f, 108.0f, 0.0f},
  {5.0f, 110.0f, 0.0f}};

GLfloat KnightBase1[10][3]={
    {0.0f, 0.0f, 0.0f},
    {30.0f,0.0f, 0.0f},
    {30.0f,5.0f, 0.0f},
    {27.0f,7.0f, 0.0f},
    {30.0f,10.0f, 0.0f},
    {20.0f,20.0f, 0.0f},
    {15.0f,25.0f, 0.0f},
    {15.0f, 30.0f, 0.0f},
    {17.0f,35.0f, 0.0f},
    {20.0f,40.0f, 0.0f}};

GLfloat KnightBase2[10][3]={
  {0.0f, 0.0f, 0.0f},
  {30.0f,0.0f, 0.0f},
  {30.0f,5.0f, 0.0f},
  {27.0f,7.0f, 0.0f},
  {30.0f,10.0f, 0.0f},
  {20.0f,20.0f, 0.0f},
  {15.0f,25.0f, 0.0f},
  {15.0f, 30.0f, 0.0f},
  {17.0f,35.0f, 0.0f},
  {20.0f,40.0f, 0.0f}};

GLfloat Knight1[15][3]={
    {10.0f,75.0f, -18.0f},
    {15.0f,73.5f, -18.0f},
    {20.0f,76.5f, -18.0f},
    {30.0f,70.0f, -18.0f},
    {35.0f,71.0f, -18.0f},
    {40.0f,75.0f, -18.0f},
    {41.0f,77.5f, -18.0f},
    {27.5f,95.0f, -18.0f},
    {25.0f,100.0f, -18.0f},
    {15.0f,102.5f, -18.0f},
    {17.5f,111.0f, -18.0f},
    {10.5f,109.0f, -18.0f},
    {7.5,106.0f, -18.0f},
    {5.0f,107.0f, -18.0f},
    {0.0f,106.0f, -18.0f}};

GLfloat Knight2[15][3]={
    {10.0f,75.0f, -18.0f},
    {15.0f,73.5f, -18.0f},
    {20.0f,76.5f, -18.0f},
    {30.0f,70.0f, -18.0f},
    {35.0f,71.0f, -18.0f},
    {40.0f,75.0f, -18.0f},
    {41.0f,77.5f, -18.0f},
    {27.5f,95.0f, -18.0f},
    {25.0f,100.0f, -18.0f},
    {15.0f,102.5f, -18.0f},
    {17.5f,111.0f, -18.0f},
    {10.5f,109.0f, -18.0f},
    {7.5,106.0f, -18.0f},
    {5.0f,107.0f, -18.0f},
    {0.0f,106.0f, -18.0f}};

GLfloat KnightBody1[14][3]={
  {20.0f,40.0f, -18.0f},
  {24.0f,45.0f, -18.0f},
  {20.0,60.0f, -18.0f},
  {16.0f,65.0f, -18.0f},
  {12.5f,70.0f, -18.0f},
  {10.0f,75.0f, -18.0f},
  {0.0f,106.0f, -18.0f},
  {-10.0f,100.0f, -18.0f},
  {-18.0f,90.0f, -18.0f},
  {-22.0f,80.5f, -18.0f},
  {-22.0f,70.0f, -18.0f},
  {-21.0f,60.0f, -18.0f},
  {-20.0f,50.0f, -18.0f},
  {-20.0f,40.0f, -18.0f}};

GLfloat KnightBody2[14][3]={
  {20.0f,40.0f, -18.0f},
  {24.0f,45.0f, -18.0f},
  {20.0,60.0f, -18.0f},
  {16.0f,65.0f, -18.0f},
  {12.5f,70.0f, -18.0f},
  {10.0f,75.0f, -18.0f},
  {0.0f,106.0f, -18.0f},
  {-10.0f,100.0f, -18.0f},
  {-18.0f,90.0f, -18.0f},
  {-22.0f,80.5f, -18.0f},
  {-22.0f,70.0f, -18.0f},
  {-21.0f,60.0f, -18.0f},
  {-20.0f,50.0f, -18.0f},
  {-20.0f,40.0f, -18.0f}};

//Drawing de Axis

void DrawAxis(void){
    glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(700.0, 0.0, 0.0);
        glColor3f(0.3f, 0.0f, 0.0f);
        glVertex3f(-700.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0, 700.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glColor3f(0.0f, 0.3f, 0.0f);
        glVertex3f(0.0, -700.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0, 0.0, 700.0);
        glVertex3f(0.0, 0.0, 0.0);
        glColor3f(0.0f, 0.0f, 0.3f);
        glVertex3f(0.0, 0.0, -700.0);
        glVertex3f(0.0, 0.0, 0.0);
    glEnd();
}

// Tranformations

void rotateObj(GLdouble angle, GLdouble rX, GLdouble rY, GLdouble rZ)
{
  double c = cos(angle);
  double s = sin(angle);
  double x2 = pow(rX,2);
  double y2 = pow(rY,2);
  double z2 = pow(rZ,2);
  double m[]={
     x2*(1.0-c)+c,        rX*rY*(1.0-c)-rZ*s,  rX*rZ*(1.0-c)+rY*s, 0.0,
     rY*rX*(1.0-c)+rZ*s,  y2*(1.0-c)+c,       rY*rZ*(1.0-c)-rX*s, 0.0,
     rX*rZ*(1.0-c)-rY*s,  rY*rZ*(1.0-c)+rX*s, z2*(1.0-c)+c,       0.0,
     0.0,                 0.0,                0.0,                1.0};
 glMultMatrixd(m);
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

void translateObj(float x,float y,float z) {
  float m[] = {
		1.0,  0.0,  0.0,  0.0,
		0.0,  1.0,  0.0,  0.0,
		0.0,  0.0,  1.0,  0.0,
		x,  y,  z, 1.0};
	glMultMatrixf(m);
}

void scaleObj(float x,float y,float z) {
  float m[] = {
		x,   0.0,  0.0,  0.0,
		0.0,  y,  0.0,  0.0,
		0.0, 0.0,  z,  0.0,
		0.0, 0.0,  0.0, 1.0};
	glMultMatrixf(m);
}

// Drawing the objects

void DrawQueen(void)
{
	int nSlices=5000;
	double dTheta=2.0*PI/nSlices;
	int nPoints=27;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*dTheta;
		double theta_next=(i+1)*dTheta;
		glColor3f(0.1f,0.1f,0.1f);
    glBegin(GL_LINE_STRIP);
		for (int j=0; j<nPoints; j++)
		{
			double x=PointsQueen[j][0];
			double y=PointsQueen[j][1];
			double z=PointsQueen[j][2];

			glVertex3f(x,y,z);

			double xNext=PointsQueen[j][0]*cos(theta_next)+PointsQueen[j][2]*sin(theta_next);
 			double yNext=PointsQueen[j][1];
			double zNext=-PointsQueen[j][0]*sin(theta_next)+PointsQueen[j][2]*cos(theta_next);

			PointsQueen2[j][0]=xNext;
			PointsQueen2[j][1]=yNext;
			PointsQueen2[j][2]=zNext;
		}
		glEnd();

		for (int j=0; j<nPoints; j++)
		{
			PointsQueen[j][0]=PointsQueen2[j][0];
			PointsQueen[j][1]=PointsQueen2[j][1];
			PointsQueen[j][2]=PointsQueen2[j][2];
		}
	}
}

void DrawKing(void)
{
	int nSlices=5000;
	double dTheta=2.0*PI/nSlices;
	int nPoints=31;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*dTheta;
		double theta_next=(i+1)*dTheta;

		glColor3f(0.1f,0.1f,0.1f);
    glBegin(GL_LINE_STRIP);
		for (int j=0; j<nPoints; j++)
		{
			double x=PointsKing[j][0];
			double y=PointsKing[j][1];
			double z=PointsKing[j][2];
			glVertex3f(x,y,z);

			double xNext=PointsKing[j][0]*cos(theta_next)+PointsKing[j][2]*sin(theta_next);
 			double yNext=PointsKing[j][1];
			double zNext=-PointsKing[j][0]*sin(theta_next)+PointsKing[j][2]*cos(theta_next);

			PointsKing2[j][0]=xNext;
			PointsKing2[j][1]=yNext;
			PointsKing2[j][2]=zNext;
		}
		glEnd();

		for (int j=0; j<nPoints; j++)
		{
			PointsKing[j][0]=PointsKing2[j][0];
			PointsKing[j][1]=PointsKing2[j][1];
			PointsKing[j][2]=PointsKing2[j][2];
		}
	}
}

void DrawRook(void)
{
	int nSlices=5000;
	double dTheta=2.0*PI/nSlices;
	int nPoints=19;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*dTheta;
		double theta_next=(i+1)*dTheta;
		glColor3f(0.1f,0.1f,0.1f);
    glBegin(GL_LINE_STRIP);
		for (int j=0; j<nPoints; j++)
		{
			double x=PointsRook[j][0];
			double y=PointsRook[j][1];
			double z=PointsRook[j][2];
     glVertex3f(x,y,z);

			double xNext=PointsRook[j][0]*cos(theta_next)+PointsRook[j][2]*sin(theta_next);
 			double yNext=PointsRook[j][1];
			double zNext=-PointsRook[j][0]*sin(theta_next)+PointsRook[j][2]*cos(theta_next);

			PointsRook2[j][0]=xNext;
			PointsRook2[j][1]=yNext;
			PointsRook2[j][2]=zNext;
		}
		glEnd();

    for (int j=0; j<nPoints; j++)
		{
			PointsRook[j][0]=PointsRook2[j][0];
			PointsRook[j][1]=PointsRook2[j][1];
			PointsRook[j][2]=PointsRook2[j][2];
		}
	}
}

void DrawPawn(void)
{
	int nSlices=5000;
	double RotationAngle=2.0*PI/nSlices;
	int nPoints=23;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*RotationAngle;
		double theta_next=(i+1)*RotationAngle;
		glColor3f(0.1f,0.1f,0.1f);
		glBegin(GL_LINE_STRIP);
		for (int j=0; j<nPoints; j++)
		{
			double x=Pawn1[j][0];
			double y=Pawn1[j][1];
			double z=Pawn1[j][2];
			glVertex3f(x,y,z);

			double xNext=Pawn1[j][0]*cos(theta_next)+Pawn1[j][2]*sin(theta_next);
 			double yNext=Pawn1[j][1];
			double zNext=-Pawn1[j][0]*sin(theta_next)+Pawn1[j][2]*cos(theta_next);

			Pawn2[j][0]=xNext;
			Pawn2[j][1]=yNext;
			Pawn2[j][2]=zNext;
		}
		glEnd();

 		for (int j=0; j<nPoints; j++)
		{
			Pawn1[j][0]=Pawn2[j][0];
			Pawn1[j][1]=Pawn2[j][1];
			Pawn1[j][2]=Pawn2[j][2];
		}
	}
}

void DrawBishop(void)
{
	int nSlices=5000;
	double RotationAngle=2.0*PI/nSlices;
	int nPoints=29;
	for (int i=0; i<nSlices;i++)
	{
		double theta=i*RotationAngle;
		double theta_next=(i+1)*RotationAngle;
	  glColor3f(0.1f,0.1f,0.1f);
		glBegin(GL_LINE_STRIP);
		for (int j=0; j<nPoints; j++)
		{
			double x=Bishop1[j][0];
			double y=Bishop1[j][1];
			double z=Bishop1[j][2];
			glVertex3f(x,y,z);

			double xNext=Bishop1[j][0]*cos(theta_next)+Bishop1[j][2]*sin(theta_next);
 			double yNext=Bishop1[j][1];
			double zNext=-Bishop1[j][0]*sin(theta_next)+Bishop1[j][2]*cos(theta_next);

			Bishop2[j][0]=xNext;
			Bishop2[j][1]=yNext;
			Bishop2[j][2]=zNext;
		}
		glEnd();

 		for (int j=0; j<nPoints; j++)
		{
			Bishop1[j][0]=Bishop2[j][0];
			Bishop1[j][1]=Bishop2[j][1];
			Bishop1[j][2]=Bishop2[j][2];
		}
	}
}

void DrawKnight(void)
{
  //Dibuja la base del caballo por Rotación
  	int nSlices=5000;
  	double RotationAngle=2.0*PI/nSlices;
  	int nPoints=10;
  	for (int i=0; i<nSlices;i++)
  	{
  		double theta=i*RotationAngle;
  		double theta_next=(i+1)*RotationAngle;
  		glColor3f(0.1f,0.1f,0.1f);
  		glBegin(GL_LINE_STRIP);
  		for (int j=0; j<nPoints; j++)
  		{
  			double x=KnightBase1[j][0];
  			double y=KnightBase1[j][1];
  			double z=KnightBase1[j][2];
  			glVertex3f(x,y,z);

  			double xNext=KnightBase1[j][0]*cos(theta_next)+KnightBase1[j][2]*sin(theta_next);
   			double yNext=KnightBase1[j][1];
  			double zNext=-KnightBase1[j][0]*sin(theta_next)+KnightBase1[j][2]*cos(theta_next);

  			KnightBase2[j][0]=xNext;
  			KnightBase2[j][1]=yNext;
  			KnightBase2[j][2]=zNext;
  		}
  		glEnd();

   		for (int j=0; j<nPoints; j++)
  		{
  			KnightBase1[j][0]=KnightBase2[j][0];
  			KnightBase1[j][1]=KnightBase2[j][1];
  			KnightBase1[j][2]=KnightBase2[j][2];
  		}
  	}

  //Dibuja la cabeza del caballo con barrido
  int nSlicesH=33;
	double dx=PI*0.0;
	double dy=PI*0.0;
	double dz=PI*0.35;
	int nPointsH=15;
	for (int i=0; i<nSlicesH;i++)
	{
    glColor3f(0.1f,0.1f,0.1f);
		glBegin(GL_POLYGON);
		for (int j=0; j<nPointsH; j++)
		{
			double x=Knight1[j][0];
			double y=Knight1[j][1];
			double z=Knight1[j][2];
			glVertex3f(x,y,z);

			double xNext=x+dx;
 			double yNext=y+dy;
			double zNext=z+dz;

			Knight2[j][0]=xNext;
			Knight2[j][1]=yNext;
			Knight2[j][2]=zNext;
		}
		glEnd();

    for (int j=1; j<nPointsH; j++)
		{
	   glBegin(GL_POLYGON);
			glVertex3f(Knight2[j][0],Knight2[j][1],Knight2[j][2]);
			glVertex3f(Knight1[j][0],Knight1[j][1],Knight1[j][2]);
			glVertex3f(Knight1[j-1][0],Knight1[j-1][1],Knight1[j-1][2]);
			glVertex3f(Knight2[j-1][0],Knight2[j-1][1],Knight2[j-1][2]);
	   glEnd();
		}

		for (int j=0; j<nPointsH; j++)
		{
			Knight1[j][0]=Knight2[j][0];
			Knight1[j][1]=Knight2[j][1];
			Knight1[j][2]=Knight2[j][2];
		}
	}

  // //Dibuja la cabeza del caballo con barrido
  int nPointsB=14;
	for (int i=0; i<nSlicesH;i++)
	{
    glColor3f(0.1f,0.1f,0.1f);
		glBegin(GL_POLYGON);
		for (int j=0; j<nPointsB; j++)
		{
			double x=KnightBody1[j][0];
			double y=KnightBody1[j][1];
			double z=KnightBody1[j][2];
			glVertex3f(x,y,z);

			double xNext=x+dx;
 			double yNext=y+dy;
			double zNext=z+dz;

			KnightBody2[j][0]=xNext;
			KnightBody2[j][1]=yNext;
			KnightBody2[j][2]=zNext;
		}
		glEnd();
    for (int j=1; j<nPointsB; j++)
		{
	   glBegin(GL_POLYGON);
			glVertex3f(KnightBody2[j][0],KnightBody2[j][1],KnightBody2[j][2]);
			glVertex3f(KnightBody1[j][0],KnightBody1[j][1],KnightBody1[j][2]);
			glVertex3f(KnightBody1[j-1][0],KnightBody1[j-1][1],KnightBody1[j-1][2]);
			glVertex3f(KnightBody2[j-1][0],KnightBody2[j-1][1],KnightBody2[j-1][2]);
	   glEnd();
		}

		for (int j=0; j<nPointsB; j++)
		{
			KnightBody1[j][0]=KnightBody2[j][0];
			KnightBody1[j][1]=KnightBody2[j][1];
			KnightBody1[j][2]=KnightBody2[j][2];
		}
	}
}

// Drawing the floor

void DrawFloor(){
  glBegin(GL_POLYGON);
    glColor3f(0.5,0.5,0.5);
    glVertex3f(0,0,0);
    glVertex3f(300,0,0);
    glVertex3f(300,0,-300);
    glVertex3f(0,0,-300);
  glEnd();
  glBegin(GL_POLYGON);
    glColor3f(0.6,0.2,1.0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,-300);
    glVertex3f(-300,0,-300);
    glVertex3f(-300,0,0);
  glEnd();
  glBegin(GL_POLYGON);
    glColor3f(0.0,0.7,0.9);
    glVertex3f(0,0,0);
    glVertex3f(-300,0,0);
    glVertex3f(-300,0,300);
    glVertex3f(0,0,300);
  glEnd();
  glBegin(GL_POLYGON);
    glColor3f(0.3,0.3,0.0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,300);
    glVertex3f(300,0,300);
    glVertex3f(300,0,0);
  glEnd();
}

/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.4f,0.8f,0.5f,1.0f); //    Set background (clear) color to black
   glNewList(king, GL_COMPILE);
      DrawKing();                       //    TODO: Draw object,
   glEndList();
   glNewList(queen, GL_COMPILE);
      DrawQueen();                       //    TODO: Draw object,
   glEndList();
   glNewList(bishop, GL_COMPILE);
      DrawBishop();                       //    TODO: Draw object,
   glEndList();
   glNewList(knight, GL_COMPILE);
      DrawKnight();                       //    TODO: Draw object,
   glEndList();
   glNewList(pawn, GL_COMPILE);
      DrawPawn();                       //    TODO: Draw object,
   glEndList();
   glNewList(rook, GL_COMPILE);
      DrawRook();                       //    TODO: Draw object,
   glEndList();
   glNewList(flr, GL_COMPILE);
      DrawFloor();                       //    TODO: Draw object,
   glEndList();
}

/* Callback handler for window re-paint event */
void display()
{
   glClear (GL_COLOR_BUFFER_BIT); /*    clear screen */
   glMatrixMode(GL_MODELVIEW);    //    To operate on the model-view matrix
   glLoadIdentity();              //    Reset model-view matrix

   gluLookAt(0.0,600.0,0.1,0.0,0.0,0.0,0.0,1.0,0.0);      //   Define camera settings

   DrawAxis();

   rotateObj(angleTheta[0], 1.0f, 0.0f, 0.0f);
   rotateObj(angleTheta[1], 0.0f, 1.0f, 0.0f);
   glCallList(flr);

   glPushMatrix();

   translateObj(-175,0,-175);
   rotateObj(qinc,0.0,1.0,0.0);
   translateObj(qinc2,0,0);
   glCallList(queen);

   glPopMatrix();
   glPushMatrix();

   translateObj(0,0,-175);
   rotateObj(kinc,1.0,0.0,0.0);
   glCallList(king);

   glPopMatrix();
   glPushMatrix();

   translateObj(175,0,-175);
   translateObj(0.0,0.0,inc_z);
   scaleObj(dec,dec,1);
   glCallList(bishop);

   glPopMatrix();
   glPushMatrix();

   translateObj(-175,0,175);
   scaleObj(sinc,sinc,1);
   glCallList(pawn);

   glPopMatrix();
   glPushMatrix();

   translateObj(0,0,175);
   rotateObj(rZx,0.0,1.0,0.0);
   shearObj(rZx,0);
   glCallList(rook);

   glPopMatrix();

   translateObj(175,0,175);
   translateObj(0.0,0.0,move_z);
   glCallList(knight);

   glFlush ();                   //   Render Object
}

// Menu

void printHelp() {
    std::cout << "\nAvailable commands:\n"
          << "press 'h' to print this message again.\n"
          << "press Esc to quit.\n"
          << "press 'k' to animate the king. \n"
      << "press 'q' to animate the queen. \n"
      << "press 'b' to animate the bishop. \n"
      << "press 'p' to animate the pawn. \n"
      << "press 'c' to animate the knight. \n"
      << "press 'r' to animate the rook. \n"
      << "press 's' to stop any animation. \n"
          << "left click to rotate in x axis\n"
      << "roght click to rotate in y axis. \n";
}

// Callbacks

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspectRatio ratio of the new window
   GLfloat aspectRatio = (GLfloat)width / (GLfloat)height; //Compute aspect ratio
   glViewport(0, 0, width, height);  // Set the viewport to cover the new screen size

   // Set the aspectRatio ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  //    To operate on the Projection matrix
   glLoadIdentity();             //    Reset the projection matrix
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
   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 15, 950.0); //    Define the shape of your viewing volume using glFrustum function

   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;
}

void timer (int value){
  if (kinc!=0){
    kinc=kinc+0.05;
  }
  if (rZx!=0){
    rZx=rZx+0.03;
  }
  if(sinc!=1){
    sinc=sinc+0.1;
  }
  if(qinc!=0){
    qinc=qinc+0.1;
  }
  if (move_z!=0.0){
    move_z= move_z-5.0;
  }
  if (inc_z!=0.0){
    inc_z=inc_z+5.0;
    dec=dec-0.01;
  }
  glutPostRedisplay();
  glutTimerFunc (200,timer,0);
}

void mouse(int btn, int state, int x, int y)
{
 if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
 if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 1;
 // if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 2;
 angleTheta[axis] += 0.02;
 // if( angleTheta[axis] <-3.0 ) angleTheta[axis] += 3.0;
 display();
}

void keyboard (unsigned char key, int x, int y)
{
  float fraction = 0.1f;
	switch (key) {
		case 'h':
			printHelp();
			break;
    case 27:  // Escape to quit
			exit(0) ;
			break ;
    case 'k':
      kinc=0.1;
      glutTimerFunc (0,timer,0);
      break;
    case 'q':
      qinc=0.1;
      qinc2=70;
      glutTimerFunc (0,timer,0);
      break;
    case 'r':
      rZx=0.05;
      glutTimerFunc(0,timer,0);
      break;
    case 'p':
      sinc=1.1;
      glutTimerFunc(0,timer,0);
      break;
    case 'c':
      move_z=0.1;
      glutTimerFunc (0,timer,0);
      break;
    case 'b':
      inc_z=0.1;
      dec=0.9;
      glutTimerFunc (0,timer,0);
      break;
    case 's':
      kinc=0;
      rZx=0;
      sinc=1;
      qinc=0;
      qinc2=0;
      move_z=0.0;
      inc_z=0.0;
      dec=1;
      break;
		default:
			break ;
	}
  display();
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("PROJECT 1 - Maria Molina & Emil Vega");      // Create window with given title
   printHelp();
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
