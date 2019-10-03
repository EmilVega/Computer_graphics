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
#include <iostream>

#define PI 3.14159265f
#define MAX_POINTS 25



float vec3DPosition[3];
float points[MAX_POINTS][3];
int ncpoints = 0;
int nSegments=20;

int matCoef[4][4]={
  {-1,3,-3,1},
  {3,-6,3,0},
  {-3,3,0,0},
  {1,0,0,0}};


// Global variables
int windowWidth = 600;  // Initial Screen and viewport width
int windowHeight = 600; // Initil Screen and viewport height

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

/* Initialize OpenGL Graphics */
void initGL()
{
    glClearColor(0.4f,0.8f,0.5f,1.0f); // TODO: Set background (clear) color to black
}

void draw(void)
{
    // Draw the point

    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);

    glBegin(GL_POINTS);
    for (int i = 0; i < ncpoints; i++)
    {
        glVertex3fv(points[i]);
    }
    glEnd();
}

void DrawAxis(void)
{
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

void drawBezierCurve()
{
    // glMap1f(GL_MAP1_VERTEX_3, 0.0, nSegments, 3, ncpoints, &points[0][0]);
    // glEnable(GL_MAP1_VERTEX_3);
    float t=0;
    float vect[4];
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<=nSegments; i++)
    {
        // glEvalCoord1f((GLfloat)i);
        t=(float)i/(float)nSegments;
        vect[0]=t*t*t;
        vect[1]=t*t;
        vect[2]=t;
        vect[3]=1;

        float mul1[4] = {0,0,0,0};
        for (int c=0;c<4; c++){

            mul1[c] = vect[0]*matCoef[0][c] +
                      vect[1]*matCoef[1][c] +
                      vect[2]*matCoef[2][c] +
                      vect[3]*matCoef[3][c];
        }
        float pT[3] = {0,0,0};
        for (int j = 0; j < 3; j++)
        {
           pT[j]=mul1[0]*points[0][j]+
                 mul1[1]*points[1][j] +
                 mul1[2]*points[2][j] +
                 mul1[3]*points[3][j];
        }
        glVertex3fv(pT);
    }
    glEnd();
}

void getMousePos3D(int x, int y, float outNewCoordinates[3])
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

    outNewCoordinates[0] = posX;
    outNewCoordinates[1] = posY;
    outNewCoordinates[2] = posZ;
}

/* Callback handler for window re-paint event */
void display()
{

    glClear(GL_COLOR_BUFFER_BIT); /* TODO: clear screen */
    glMatrixMode(GL_MODELVIEW); // TODO: To operate on the model-view matrix
    glLoadIdentity();           // TODO: Reset model-view matrix

    gluLookAt(0.0,0.0,500,  0.0,0.0,0.0,   0.0,1.0,0.0);

    DrawAxis();
    glColor3f(0.0, 0.0, 1.0);
    std::cout<<"Puntos: "<<ncpoints<<"\n";
    if (ncpoints==4) drawBezierCurve();
    draw();
    glFlush(); //TODO: Render Object
}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height)
{
    // Compute aspectRatio ratio of the new window
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height; //Compute aspect ratio

    glViewport(0, 0, width, height); // Set the viewport to cover the new screen size

    // Set the aspectRatio ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); // TODO: To operate on the Projection matrix
    glLoadIdentity();            // TODO: Reset the projection matrix
    if (width >= height)
    {
        windowAreaXLeft = -windowWidth * aspectRatio;
        windowAreaXRight = windowWidth * aspectRatio;
        windowAreaYBottom = -windowHeight;
        windowAreaYTop = windowHeight;
    }
    else
    {
        windowAreaXLeft = -windowWidth;
        windowAreaXRight = windowWidth;
        windowAreaYBottom = -windowHeight / aspectRatio;
        windowAreaYTop = windowHeight / aspectRatio;
    }

    //Adjust vision cone
    //gluOrtho2D(windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop);
    //glFrustum(0.0,10.0,0.0,10.0,5.0,500.0);

    //gluPerspective(90.0, GLfloat(width) / GLfloat(height), 0.5, 200.0); //   Define the shape of your viewing volume using glFrustum function
    glOrtho(windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop, -(windowAreaXRight - windowAreaXLeft) / 2, (windowAreaXRight - windowAreaXLeft) / 2);
    //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
    windowXMin = windowAreaXLeft;
    windowXMax = windowAreaXRight;
    windowYMin = windowAreaYBottom;
    windowYMax = windowAreaYTop;
}


void mouse(int btn, int state, int x, int y)
{
  if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    getMousePos3D(x, y, vec3DPosition);

    points[ncpoints][0] = vec3DPosition[0];
    points[ncpoints][1] = vec3DPosition[1];
    points[ncpoints][2] = vec3DPosition[2];
    ncpoints++;
    glutPostRedisplay();
  }

}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                         // Initialize GLUT
    glutInitWindowSize(windowWidth, windowHeight); // Initial window width and height
    glutCreateWindow("Bezier Curve");                   // Create window with given title
    glutDisplayFunc(display);                      // Register callback handler for window re-paint
    glutReshapeFunc(reshape);                      // Register callback handler for window re-shape
    //glutKeyboardFunc(mykey);                       /* TODO keyboard callback invoked when mouse is used */
    glutMouseFunc(mouse);
    initGL();       // Our own OpenGL initialization
    glutMainLoop(); // Enter event-processing loop
    return 1;
}
