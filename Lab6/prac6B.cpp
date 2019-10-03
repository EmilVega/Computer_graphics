/****************************************************************************/
/* Prac. 5A Polygonal mesh with triangles                                        */
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

#include "load-mesh.h"

#define PI 3.14159265f

#define Obj1 1
#define Obj2 2

// Global variables
int windowWidth  = 500;     // Initial Screen and viewport width
int windowHeight = 500;     // Initil Screen and viewport height

GLfloat zValue=500;
 GLfloat xRotated, yRotated, zRotated;
 static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 2;
int slices = 10;

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;


#define rX 100
#define rY 40
#define rZ 20

#define r 10
#define R 60

int nlatitude=20;
int nlongitude=40;



// GLfloat r = pow(X,2) + pow(Y,2)
static GLfloat ChosenKey[] = {0.0,0.0};

static GLint sc = 10;
static GLfloat zX = 0;

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

/*normalize a vector*/
void normalize(float v[3]) {
   GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
   if (d == 0.0) {
      printf("\nErrot: zero length vector");
      return;
   }
   v[0] /= d; v[1] /= d; v[2] /= d;
}

//v1[], v2[] are two vectors
//out[] holds the crossproduct v1 x v2
void normcrossprod(float v1[3], float v2[3], float out[3])
{
   GLint i, j;
   GLfloat length;

   out[0] = v1[1]*v2[2] - v1[2]*v2[1];
   out[1] = v1[2]*v2[0] - v1[0]*v2[2];
   out[2] = v1[0]*v2[1] - v1[1]*v2[0];
   normalize(out);
}

// /*Draw object*/

void shearObj(float Shx, float Shy)
{
  float m[]={
     1.0,   Shy,  0.0, 0.0,
     Shx,  1.0,   0.0, 0.0,
     0.0,   0.0,    1.0, 0.0,
     0.0,   0.0,    0.0, 1.0};
 glMultMatrixf(m);
}

void DrawObj(){
  int nlatitude=20;
  int nlongitude=20;
  GLfloat varSlicesLatitude=2*PI/nlatitude;
  GLfloat varSlicesLongitude=2*PI/nlongitude;
  float vertex [3];
  for (int i=0; i<=nlatitude; i++){
    glBegin(GL_TRIANGLE_STRIP);
      for(int j=0; j<=nlongitude ; j++){
        vertex[0]=r*cos((i+1)*varSlicesLatitude);
        vertex[1]=r*sin((i+1)*varSlicesLatitude);
        vertex[2]=j*varSlicesLongitude;

        glVertex3fv(vertex);

        vertex[0]=r*cos(i*varSlicesLatitude);
        vertex[1]=r*sin(i*varSlicesLatitude);

        glVertex3fv(vertex);

      }
    glEnd();
  }
}

void DrawObj2(){
  int nlatitude=20;
  int nlongitude=40;
  GLfloat varSlicesLatitude=2*PI/nlatitude;
  GLfloat varSlicesLongitude=2*PI/nlongitude;
  float vertex [3];
  for (int i=0; i<=nlatitude; i++){
    glBegin(GL_TRIANGLE_STRIP);
      for(int j=0; j<=nlongitude ; j++){
        vertex[0]=(i+1)*varSlicesLatitude*cos(j*varSlicesLatitude);
        vertex[1]=(i+1)*varSlicesLatitude*sin(j*varSlicesLatitude);
        vertex[2]=(i+1)*varSlicesLatitude;

        glVertex3fv(vertex);

        vertex[0]=i*varSlicesLatitude*cos(j*varSlicesLatitude);
        vertex[1]=i*varSlicesLatitude*sin(j*varSlicesLatitude);
        vertex[2]=i*varSlicesLatitude;

        glVertex3fv(vertex);

      }
    glEnd();
  }
}


/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.4f,0.8f,0.5f,1.0f); //   Set background (clear) color to black
   // load_mesh("object3DTriangleMesh.obj");
   // glEnable(GL_LIGHTING);   //TODO: UNCOMMENT
   // glEnable(GL_LIGHT0);     //TODO: UNCOMMENT
   glNewList(Obj1, GL_COMPILE);
    DrawObj();        //    Draw a object,
   glEndList();
   glNewList(Obj2, GL_COMPILE);
    DrawObj2();        //    Draw a object,
   glEndList();
   glPolygonMode(GL_FRONT, GL_LINE);
   glPolygonMode(GL_BACK, GL_LINE);

}

/* Callback handler for window re-paint event */
void display()
{

   glClear (GL_COLOR_BUFFER_BIT); /*   clear screen */
   glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
   glLoadIdentity();              //   Reset model-view matrix


   gluLookAt(0.0,0.0,50,  0.0,0.0,0.0,   0.0,1.0,0.0);      //  Define camera settings

    DrawAxis();


    // shearObj(zX,0.0);
    glRotatef(angleTheta[0], 1.0, 0.0, 0.0);


    // glTranslatef(ChosenKey[0], 0, 0);
    glPushMatrix();

    glTranslatef(-13, 0, 0);
    glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 1.0f);
    glCallList(Obj1);

    glPopMatrix();

    glTranslatef(10, 0, 0);
    glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 1.0f);
    glCallList(Obj2);


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

   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 0.5, 200.0); //   Define the shape of your viewing volume using glFrustum function



   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;


}

void keyboard(unsigned char key, int x, int y)
{
switch (key) {
		case 'r':
			ChosenKey[0]+=1.0;
		break;
		case 'l':
			ChosenKey[1]-=1.0;
		break;
    case 's':
			sc+=1;
		break;
    case 'a':
			sc-=1;
		break;
    case 'z':
			zX+=0.05;
		break;
		default:
		break;
	}
  display();
}

 void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
        if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 1;
	if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 2;
	angleTheta[axis] += 2.0;
	if( angleTheta[axis] > 360.0 ) angleTheta[axis] -= 360.0;
	display();
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("Prac 5A");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
