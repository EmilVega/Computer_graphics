/****************************************************************************/
/* MIDTERM - QUESTION 2
Estudiante: Emil Darío Vega Gualán                                         */
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

#include "load-mesh.h"
#include "load-mesh2.h"

#define PI 3.14159265f

#define objBun 1
#define objX 2


float mot1=0;
float mot2=0;

// Global variables
int windowWidth  = 500;     // Initial Screen and viewport width
int windowHeight = 500;     // Initil Screen and viewport height

GLfloat zValue=500;
 GLfloat xRotated, yRotated, zRotated;
 static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 0;
int slices = 10;

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

//sqrt(X² + Y²) = r
#define X 50
#define Z 80

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

  int pos0, pos1, pos2;
  GLfloat vecA[3], vecB[3], norm[3];
  glBegin(GL_TRIANGLES);
    for (int i=0; i<tIndices.size();i++){

      pos0=tIndices[i].indices[0];
      pos1=tIndices[i].indices[1];
      pos2=tIndices[i].indices[2];

      vecA[0]=vVertex[pos0].x - vVertex[pos1].x;
      vecA[1]=vVertex[pos0].y - vVertex[pos1].y;
      vecA[2]=vVertex[pos0].z - vVertex[pos1].z;

      vecB[0]=vVertex[pos2].x - vVertex[pos1].x;
      vecB[1]=vVertex[pos2].y - vVertex[pos1].y;
      vecB[2]=vVertex[pos2].z - vVertex[pos1].z;

      normcrossprod(vecA, vecB, norm);
      glNormal3fv(norm);
      glVertex3f(vVertex[pos0].x, vVertex[pos0].y, vVertex[pos0].z);
      glNormal3fv(norm);
      glVertex3f(vVertex[pos1].x, vVertex[pos1].y, vVertex[pos1].z);
      glNormal3fv(norm);
      glVertex3f(vVertex[pos2].x, vVertex[pos2].y, vVertex[pos2].z);
    }
  glEnd();

}

void DrawObj2(){

  int pos0, pos1, pos2;
  GLfloat vecA[3], vecB[3], norm[3];
  glBegin(GL_TRIANGLES);
    for (int i=0; i<tIndices2.size();i++){

      pos0=tIndices2[i].indices[0];
      pos1=tIndices2[i].indices[1];
      pos2=tIndices2[i].indices[2];

      vecA[0]=vVertex2[pos0].x - vVertex2[pos1].x;
      vecA[1]=vVertex2[pos0].y - vVertex2[pos1].y;
      vecA[2]=vVertex2[pos0].z - vVertex2[pos1].z;

      vecB[0]=vVertex2[pos2].x - vVertex2[pos1].x;
      vecB[1]=vVertex2[pos2].y - vVertex2[pos1].y;
      vecB[2]=vVertex2[pos2].z - vVertex2[pos1].z;

      normcrossprod(vecA, vecB, norm);
      glNormal3fv(norm);
      glVertex3f(vVertex2[pos0].x, vVertex2[pos0].y, vVertex2[pos0].z);
      glNormal3fv(norm);
      glVertex3f(vVertex2[pos1].x, vVertex2[pos1].y, vVertex2[pos1].z);
      glNormal3fv(norm);
      glVertex3f(vVertex2[pos2].x, vVertex2[pos2].y, vVertex2[pos2].z);
    }
  glEnd();

}



/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.4f,0.8f,0.5f,1.0f); //   Set background (clear) color to black


   glNewList(objX, GL_COMPILE);
    load_mesh("objT_midterm.obj");
    DrawObj();        //    Draw a object,
   glEndList();


   glNewList(objBun, GL_COMPILE);
    load_mesh2("object3DTriangleMesh.obj");
    DrawObj2();        //    Draw a object,
   glEndList();

}


/* Callback handler for window re-paint event */
void display()
{

   glClear (GL_COLOR_BUFFER_BIT); /*   clear screen */
   glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
   glLoadIdentity();              //   Reset model-view matrix


   gluLookAt(0.0,0.0,7,  0.0,0.0,0.0,   0.0,1.0,0.0);      //  Define camera settings

   DrawAxis();
   glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
   glRotatef(angleTheta[1], 0.0, 1.0, 0.0);

   glPushMatrix();
    shearObj(zX,0);

    glTranslatef(mot1, 0, 0);
    glTranslatef(2, 0, 0);

   /* Draw obj */
    glColor3f(0.0f, 0.0f, 1.0f);
    glScalef(10,10,10);
    glCallList(objBun);

   // glCallList(ObjIcosahedron);        //   Draw obj
   glPopMatrix();

   // glClear (GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0f, 0.0f, 0.0f);
   // glScalef(-3000, -3000, -3000);
   glTranslatef(mot2, 0, 0);
   glTranslatef(-2, 0, 0);
   glCallList(objX);

   // printf("%s\n", );

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

   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 0.5, 500.0); //   Define the shape of your viewing volume using glFrustum function



   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;


}

void timer (int value){
  if(mot1==-1.2|| mot2==1.2){
    mot1=mot1; mot2=mot2;
  }else { if(mot1<=2){
    mot1-=0.3;}
    if(mot2>=-2){
      mot2+=0.3;}
  }

  glutPostRedisplay();
  glutTimerFunc (500,timer,0);
}


void keyboard(unsigned char key, int x, int y)
{
switch (key) {
		case 'm':
      mot1=-0.3;
      mot2=0.3;
			glutTimerFunc (0,timer,0);
		break;
		case 'l':
      glEnable(GL_LIGHTING);   //TODO: UNCOMMENT
      glEnable(GL_LIGHT0);     //TODO: UNCOMMENT
		break;
    case 'q':
      glDisable(GL_LIGHTING);   //TODO: UNCOMMENT
      glDisable(GL_LIGHT0);     //TODO: UNCOMMENT
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
   glutCreateWindow("Midterm Question 3");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
