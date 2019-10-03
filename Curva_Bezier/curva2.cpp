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
int windowWidth  = 500;     // Initial Screen and viewport width
int windowHeight = 500;     // Initil Screen and viewport height
float outNewCoordinates[3];
GLfloat P[4][3];

int cont = -1;
GLfloat Coef[4][4] ={
    {-1.0,3.0,-3.0,1.0},
    {3.0,-6.0,3.0,0.0},
    {-3.0,3.0,0.0,0.0},
    {1.0,0.0,0.0,0.0}
};
GLfloat zValue=500;
 GLfloat xRotated, yRotated, zRotated;
 static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 2;
int slices = 10;
GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;


void Bezier(){
    int nSegments = 10;
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i<=nSegments+1;i++){
        GLfloat t = (float)i/(float)nSegments;
        GLfloat Vect[4];
        Vect[0] = t*t*t;
        Vect[1] = t*t;
        Vect[2] = t;
        Vect[3] = 1;
        GLfloat tCoef[4] = {0,0,0,0};
        for(int j = 0 ; j<4;j++){
            GLfloat val = 0;
            for(int i = 0 ; i<4;i++){
                val = val + Vect[i] * Coef[i][j];
            }
            tCoef[j] = val;
        }
        GLfloat tCoefP[3] = {0,0,0};
        for(int j = 0 ; j<3;j++){
            GLfloat val = 0;
            for(int i = 0 ; i<4 ;i++){
                val = val + tCoef[i] * P[i][j];
            }
            tCoefP[j] = val;
        }
        glVertex3fv(tCoefP);
    }
    glEnd();

}

// void GetMousePos3D(int x, inty, float outNewCoordinates[3])
// {
//     GLint viewport[4];
//     GLdouble modelview[16];
//     GLdouble projection[16];

//     GLfloat winX, winY, winZ;
//     GLdouble posX, posY, posZ;

//     glGetDoublev( GL_MODELVIEW_MATRIX, modelview);
//     glGetDoublev( GL_PROJECTION_MATRIX, projection);
//     glGetIntegerv(GL_VIEWPORT, viewport);

//     winX = (float)x;
//     winY = (float)viewport[3] - (float)y-1;
//     glReadPixels( x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

//     gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

//     P[cont][0] = posX;
//     P[cont][1] = posY;
//     P[cont][2] = posZ;
// }

void DrawAxis(void){
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

// void DrawBezierCurve()
// {
//     glMap1f(GL_MAP1_VERTEX_3, 0.0, 30.0, 3, ncpts, &cpts[0][0]);
//     glEnable(GL_MAP1_VERTEX_3);
//     glBegin(GL_LINE_STRIP);
//         for(int i=0; i<=30, i++)
//         {
//                 glEvalCoord1f((GLfloat)i);
//         }
//     glEnd();
// glFlush();
// }

/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.4f,0.8f,0.5f,1.0f); //   Set background (clear) color to black

   //glEnable(GL_LIGHTING);   //TODO: UNCOMMENT
   //glEnable(GL_LIGHT0);     //TODO: UNCOMMENT

}

/* Callback handler for window re-paint event */
void display()
{

   glClear (GL_COLOR_BUFFER_BIT); /*   clear screen */
   glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
   glLoadIdentity();              //   Reset model-view matrix


   gluLookAt(0.0,0.0,zValue,  0.0,0.0,0.0,   0.0,1.0,0.0);      //  Define camera settings
   glPushMatrix();
    glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
    glRotatef(angleTheta[1], 0.0, 1.0, 0.0);

   //DrawBezierCurve();
   glColor3f(1.0, 1.0, 0.0);
   if(cont == 3)Bezier();
   glColor3f(1.0, 0.0, 0.0);
   glPointSize(5.0);
   for (int i=0;i<=cont;i++){
        glBegin(GL_POINTS);
            glVertex3fv(P[i]);
        glEnd();

   }
   glPopMatrix();

   DrawAxis();
   glFlush ();                   //  Render Object
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

   //gluPerspective(60.0, GLfloat(width) / GLfloat(height), 0.5, 500.0); //   Define the shape of your viewing volume using glFrustum function


   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;

   glOrtho(windowXMin, windowXMax, windowYMin, windowYMax, -1*(windowXMax -windowXMin)/2, (windowXMax-windowXMin)/2);


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
    winY = (float)viewport[3] - (float)y-1;
    glReadPixels( x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    P[cont][0] = posX;
    P[cont][1] = posY;
    P[cont][2] = posZ;
    /* display(); */
}

 void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(cont<3)
            cont++;
        GetMousePos3D(x,y);
    }
	display();
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("Bezier Curve");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
