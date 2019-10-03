/****************************************************************************/
/* Prac. 6A Polygonal mesh with triangles, load obj file                    */
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

#include "load-mesh.h"

#define PI 3.14159265f
#define ObjBunnyMesh 1
#define LightSphere 2

#define X 50
#define Z 80
// static GLfloat vVertex[12][3] =
//     {
//     {-X,0.0,Z}, {X,0.0,Z}, {-X,0.0,-Z}, {X,0.0,-Z},
//     {0.0,Z,X}, {0.0,Z,-X}, {0.0,-Z,X}, {0.0,-Z,-X},
//     {Z,X,0.0}, {-Z,X,0.0}, {Z,-X,0.0}, {-Z,-X,0.0},
//     };

// static GLuint tindices[20][3] =
//     {
//     {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
//     {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
//     {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
//     {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
//     };

//----------------------LUZ FOCO
GLfloat rL = 5;
GLfloat angulo = PI / 2;
GLfloat r = 5;
GLfloat zL = r * sin(angulo);
GLfloat xL= r * cos(angulo);
GLfloat yL = 5;

GLfloat light[3] = { xL,yL,zL };
GLfloat m[16] = { 1,0,0,0,0,1,0,-1 / yL,0,0,1,0,0,0,0,0 };
bool loco = false;

typedef struct _LightInfo {
	GLfloat xyz[4];
	GLfloat *rgb;
	int enable;
} LightInfo;

typedef struct _LightBrightness {
	int num;
	GLfloat brighteness;
} LightBrightness;

//DEFINE MATERIAL MODEL
static GLfloat modelAmb[4] = { 0.1, 0.1, 0.1, 1.0 };
static GLfloat matAmb[4] = { 0.9, 0.9, 0.9, 1.0 };
static GLfloat matDiff[4] = { 0.8, 0.8, 0.8, 1.0 };
static GLfloat matSpec[4] = { 0.9, 0.9, 0.9, 1.0 };
static GLfloat matEmission[4] = { 0.0, 0.0, 0.0, 1.0 };

//DEFINE COLOR LIGHT
GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat dim[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat celeste[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat rojo[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat magenta[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat indigo[] = { 75.0/255.0, 0.0, 130.0/255.0, 1.0 };



//HOW MANY LIGHT
int lightState[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

LightInfo linfo[] = {
	{{1.0, 1.0, -0.7, 1.0}, rojo},
    {{0.0, 1.0, -1.2, 1.0}, yellow},
    {{-1.0, 3.0, 0.0, 1.0}, green},
	{{0.0, 1.0, 1.5, 1.0}, celeste},
	{{1.0, 1.0, 0.8, 1.0}, magenta},
	{{-2.0, 1.0, 0.0, 1.0}, indigo}
    //{{-2.0, -3.0, 0.0, 1.0}, yellow}
};

static int numActiveLights;
#define MAX_LIGHTS (sizeof(linfo)/sizeof(linfo[0]))
#define MIN_VALUE(a,b) (((a)<(b))?(a):(b))
#define MAX_VALUE(a,b) (((a)>(b))?(a):(b))

void initLight(int num) {
	glLightf(GL_LIGHT0 + num, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0 + num, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0 + num, GL_QUADRATIC_ATTENUATION, 0.1);
	glLightfv(GL_LIGHT0 + num, GL_SPECULAR, dim);
}


// Global variables
int windowWidth  = 500;     // Initial Screen and viewport width
int windowHeight = 500;     // Initil Screen and viewport height

GLfloat zValue=7;
//GLfloat zValue=7;
 GLfloat xRotated, yRotated, zRotated;
 static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 2;
int factorSize = 20;
GLfloat radio = 20.0f;
double alpha=PI/2, beta=PI/2;
GLfloat zCamara = radio * cos(beta)*sin(alpha);
GLfloat xCamara = radio * cos(beta)*cos(alpha);
GLfloat yCamara = radio * sin(beta);

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;


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

/*
	Your display function should look roughly like the following.
*/
void DrawBunnyObj()
{
        int k0,k1,k2;

      //   glPolygonMode(GL_FRONT, GL_LINE);
      //   glPolygonMode(GL_BACK, GL_LINE);
    	for (int i = 0; i < tIndices.size(); i++)
	{
	  /* color information here */
	  //Normal information
	  /*
	  Note: difference between two points is a vector
	  i.e. 	A = P0 - P1
	 	B = P2 -  p1
		A x B is normal to the plane passing through points P0, P1, P2,
		and P0, P1, P2 appear counter-clockwise
	  ( see notes above )
	 */
	  k0 = tIndices[i].indices[0];
	  k1 = tIndices[i].indices[1];
	  k2 = tIndices[i].indices[2];

	  GLfloat vecA[3], vecB[3], norm[3];

	  vecA[0] = vVertex[k0].x - vVertex[k1].x;
	  vecA[1] = vVertex[k0].y - vVertex[k1].y;
	  vecA[2] = vVertex[k0].z - vVertex[k1].z;
	  //vecB = vVertex[k2]- vVertex[k1];
	  vecB[0] = vVertex[k2].x - vVertex[k1].x;
	  vecB[1] = vVertex[k2].y - vVertex[k1].y;
	  vecB[2] = vVertex[k2].z - vVertex[k1].z;

	   normcrossprod(vecA, vecB, norm);

	glBegin(GL_POLYGON);
	   //vertex information
	   //glNormal3f(gNormals[k0].x,gNormals[k0].y,gNormals[k0].z);
           float out[3];
           out[0]=vVertex[k0].x; out[1]=vVertex[k0].y; out[2]=vVertex[k0].z;
	   glNormal3fv(norm);
           glVertex3fv( out );

	  //glNormal3f(gNormals[k1].x,gNormals[k1].y,gNormals[k1].z);

	   out[0]=vVertex[k1].x; out[1]=vVertex[k1].y; out[2]=vVertex[k1].z;
           glNormal3fv(norm);
	    glVertex3fv( out );


	   out[0]=vVertex[k2].x; out[1]=vVertex[k2].y; out[2]=vVertex[k2].z;
           glNormal3fv(norm);
	   glVertex3fv( out );
           //printf("%f, %f", vVertex[k2].x,vVertex[k0].y);
         glEnd();   // End Drawing Obj
   	}


}
void Tablero(float ini, float fin, int div) {
	float len = (fin - ini) / div;
	GLfloat posx, posz = ini;
	for (int i = 0; i < div; i++) {
		posx = ini;
		for (int j = 0; j < div; j++) {
			if (loco) {
				glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);
			}
			else
			{
				if (j % 2 == 0) {
					if (i % 2 == 0) {
						glColor3f(0.0f, 0.5f, 0.5f);
					}
					else {
						glColor3f(0.5f, 0.5f, 0.0f);
					}
				}
				else {
					if (i % 2 != 0) {
						glColor3f(0.0f, 0.5f, 0.5f);
					}
					else {
						glColor3f(0.5f, 0.5f, 0.0f);
					}
				}
			}
			glBegin(GL_POLYGON);
			glVertex3f(posx, 0.0f, posz);
			//glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);

			glVertex3f(posx, 0.0f, posz + len);
			//glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);

			glVertex3f(posx + len, 0.0f, posz + len);
			//glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);

			glVertex3f(posx + len, 0.0f, posz);
			glEnd();
			posx = posx + len;
		}
		posz = posz + len;
	}
}

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

/* Initialize OpenGL Graphics */
void initGL()
{
   numActiveLights = MIN_VALUE(MAX_LIGHTS, 8);
	for (int i = 0; i < numActiveLights; i++) {
		initLight(i);
	}
   glClear(GL_COLOR_BUFFER_BIT);
   glClearColor(0.0, 0.0, 0.0, 0.0);

   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, modelAmb);
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

   glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb );
   glMaterialfv(GL_FRONT, GL_DIFFUSE , matDiff );
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec );
   glMaterialfv(GL_FRONT, GL_EMISSION, matEmission );
   glMaterialf(GL_FRONT, GL_SHININESS, 100.0); //entre 100 y 500, menores a 100 superficies más brillantes, mayores a 100 superficies más oscuras

   //glClearColor(0.0, 0.6, 0.0, 1.0); //   Set background (clear) color to black
   load_mesh("object3DTriangleMesh.obj");
	 // load_mesh("teapot.obj");

   glNewList(ObjBunnyMesh, GL_COMPILE);
      DrawBunnyObj();       //    Draw a object,
   glEndList();
   glNewList(LightSphere, GL_COMPILE);
   glutSolidSphere(0.2, 4, 4);
   glEndList();


}
void drawLight(LightInfo *info) {
	glPushMatrix();
	glTranslatef(info->xyz[0], info->xyz[1], info->xyz[2]);
	glColor3fv(info->rgb);
	glCallList(LightSphere);
	glPopMatrix();
}
void drawFoco() {
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glTranslatef(-xL*(2), -yL*2, -zL*2);
	glScalef(3, 3, 3);
	glTranslatef(xL, yL, zL);
	glCallList(LightSphere);
	glPopMatrix();

}

/* Callback handler for window re-paint event */
void display()
{
   glClear (GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(xCamara,yCamara,zCamara,  0.0,0.0,0.0,   0.0,1.0,0.0);

   glPushMatrix();
   glDisable(GL_LIGHTING);
   Tablero(-200.0, 200.0, 200.0);
   glPopMatrix();
   glPushMatrix();
   glDisable(GL_LIGHTING);
   drawFoco();
   for (int i = 0; i < MAX_LIGHTS; i++) {
	   drawLight(&linfo[i]);
   }
   glEnable(GL_LIGHTING);
   for (int i = 0; i < numActiveLights; i++) {
	   if (lightState[i]) {
		   int num = i; //ld[i].num;
		   glLightfv(GL_LIGHT0 + i, GL_POSITION, linfo[num].xyz);
		   glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, linfo[num].rgb);
		   glEnable(GL_LIGHT0 + i);
	   }
	   else {
		   glDisable(GL_LIGHT0 + i);
	   }
   }




        //  Define camera settings
   glDisable(GL_LIGHTING);
   glPushMatrix();
   glTranslatef(xL,yL,zL);
   glMultMatrixf(m);
   glTranslatef(-xL,-yL,-zL);
   if (loco) {
	glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);
   }
   else {
	glColor3f(0.1,0.1,0.1);
   }
   glScalef(20,20,20);
	 // glScalef(-1,-1,-1);
   glCallList(ObjBunnyMesh);
   glPopMatrix();
   glPushMatrix();
   glTranslatef(xL, yL, zL);
   glMultMatrixf(m);
   glTranslatef(-xL, -yL, -zL);
   if (loco) {
	   glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);
   }
   else {
	   glColor3f(0.1, 0.1, 0.1);
   }
   glScalef(10, 10, 10);
   glTranslatef(0.2, 0.0, 0);
   glCallList(ObjBunnyMesh);
   glPopMatrix();


   glEnable(GL_LIGHTING);

   glPushMatrix();
   glColor3f(0.3f,0.7f,0.0f);
   glScalef(20,20,20);
   glTranslatef(0,-0.0345,0);
   glCallList(ObjBunnyMesh);
   glPopMatrix();

   glPushMatrix();
   glColor3f(0.3f, 0.7f, 0.0f);
   glScalef(10, 10, 10);
   glTranslatef(0.2, -0.0345, 0);
   glCallList(ObjBunnyMesh);
   glPopMatrix();

   glPopMatrix();

   glDisable(GL_LIGHTING);
   DrawAxis();
   glutSwapBuffers();


   //glFlush ();                   //  Render Object
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

   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 0.5, 500.0); //   Define the shape of your viewing volume using glFrustum function



   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;


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

void moverCamara(unsigned char k, int x, int y) {
	switch (k) {
	case '4':
		alpha = alpha + 0.07;
		break;
	case '6':
		alpha = alpha - 0.07;
		//beta = beta + 0.01;
		break;
	case '2':
		beta = beta + 0.05;
		break;
	case '8':
		beta = beta - 0.05;
		break;
	case '1':
		radio = radio - 0.5;
		break;
	case '3':
		radio = radio + 0.5;
		break;
   case 'r':
       //printf("NAL: %d \n", numActiveLights);
       if(lightState[0]==1){
          lightState[0]=0;
       } else{
          lightState[0]=1;
       }
      break;
   case 'o':
      for (int i = 0; i < numActiveLights; i++) {
         lightState[i] = 0;
      }
      break;
   case 'p':
      for (int i = 0; i < numActiveLights; i++) {
         lightState[i] = 1;
      }
      break;
   case 'v':
       if(lightState[2]==1){
          lightState[2]=0;
       } else{
          lightState[2]=1;
       }
      break;
   case 'a':
       if(lightState[1]==1){
          lightState[1]=0;
       } else{
          lightState[1]=1;
       }
      break;
   case 'd':
       if(lightState[3]==1){
          lightState[3]==0;
       } else{
          lightState[3]==1;
       }
      break;
   case '5':
	   angulo = angulo + 0.05;
	   break;
   case '0':
	   loco = !loco;
	default:
		break;
	}
	zL = r * sin(angulo);
	xL = r * cos(angulo);
	zCamara = radio * cos(beta)*sin(alpha);
	xCamara = radio * cos(beta)*cos(alpha);
	yCamara = radio * sin(beta);
	display();
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("Final");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);
   //glutMouseFunc(mouse);
   glutKeyboardFunc(moverCamara);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
