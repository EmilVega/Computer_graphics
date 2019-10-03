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

#define PI 3.14159265f

#define ObjIcosahedron 1
#define DL_LIGHT_SPHERE 2

// Global variables and define functions for lights

static int numActiveLights;

#define MAX_LIGHTS (sizeof(linfo)/sizeof(linfo[0]))
#define MIN_VALUE(a,b) (((a)<(b))?(a):(b))
#define MAX_VALUE(a,b) (((a)>(b))?(a):(b))

// Global variables
int windowWidth  = 500;     // Initial Screen and viewport width
int windowHeight = 500;     // Initil Screen and viewport height

GLfloat zValue=7;
 GLfloat xRotated, yRotated, zRotated;
  GLfloat angleTheta[] = {0.0,0.0,0.0};
 GLint axis = 2;
int slices = 10;

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;


#define X 50 //TODO: Set the value of X
#define Z 80 //TODO: Set the value of Z
GLfloat r = sqrt(pow(X,2) + pow(Z,2));

GLfloat vVertex[12][3] =
    {
    {-X,0.0,Z}, {X,0.0,Z}, {-X,0.0,-Z}, {X,0.0,-Z},
    {0.0,Z,X}, {0.0,Z,-X}, {0.0,-Z,X}, {0.0,-Z,-X},
    {Z,X,0.0}, {-Z,X,0.0}, {Z,-X,0.0}, {-Z,-X,0.0},
    };

GLuint tindices[20][3] =
    {
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
    };


typedef struct _LightInfo{
  GLfloat xyz[4];
  GLfloat *rgb;
  int enable ;
} LightInfo;

typedef struct _LightBrightness{
  int num;
  GLfloat brightness;
} LighBrightness;

// define material model //

static GLfloat modelAmb[4]={0.1,0.1,0.1,1.0};
static GLfloat matAmb[4]={0.9,0.9,0.9,1.0};
static GLfloat matDiff[4]={0.8,0.8,0.8,1.0};
static GLfloat matSpec[4]={0.9,0.9,0.9,1.0};
static GLfloat matEmission[4]={0.0,0.0,0.0,1.0};

// Define colo Light //

GLfloat green[]={0.0,1.0,0.0,1.0};
GLfloat red[]={1.0,0.0,0.0,1.0};
GLfloat blue[]={0.0,0.0,1.0,1.0};
GLfloat yellow[]={1.0,1.0,0.0,1.0};
GLfloat magenta[]={1.0,0.0,1.0,1.0};
GLfloat white[]={1.0,1.0,1.0,1.0};

GLfloat dim[]={0.5,0.5,0.5,1.0};

// How many light //

int lightState[8]={1,1,1,1,1,1,1,1};

LightInfo linfo[]={
  {{0.0,6.0,-5.0,1.0}, green},
  {{-4.0,2.0,-2.0,1.0}, red},
  {{4.0,0.0,-6.0,1.0}, magenta},
  {{1.0,-1.0,3.0,1.0}, white},
  {{-2.0,-2.0,2.0,1.0}, yellow},
  {{-3.0,0.0,2.0,1.0}, blue},
  {{3.0,1.0,2.0,1.0}, red},
};



/* Draw a sphere the same color as the light at the light position so it is
easy to tell where the positional light source are. */

void drawLight(LightInfo * info){
  glPushMatrix();
  glTranslatef(info->xyz[0], info->xyz[1], info->xyz[2]);
  glColor3fv(info->rgb);
  glCallList(DL_LIGHT_SPHERE);
  glPopMatrix();
}

void initLight(int num){
  glLightf(GL_LIGHT0 + num, GL_CONSTANT_ATTENUATION, 0.0);
  glLightf(GL_LIGHT0 + num, GL_LINEAR_ATTENUATION, 0.0);
  glLightf(GL_LIGHT0 + num, GL_QUADRATIC_ATTENUATION, 0.1);
  glLightfv(GL_LIGHT0 + num, GL_SPECULAR, dim);
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

/*normalize a vector*/
void normalize(float v[3]) {
   GLfloat d = sqrt(v[0]*v[0] + v[1]*v[1]+v[2]*v[2]);            //TODO: compute the magnitude of the vector
   if (d == 0.0) {
      printf("\nErrot: zero length vector");
      return;
   }
   v[0] =  v[0]/d ; v[1] = v[1]/d ; v[2]= v[2]/d ;    //TODO: normalize the vector
}

//v1[], v2[] are two vectors
//out[] holds the crossproduct v1 x v2
void normcrossprod(float v1[3], float v2[3], float out[3])
{
   GLint i, j;
   GLfloat length;

    out[0] =  v1[1]*v2[2] - v1[2]*v2[1];                                    //TODO: Compute the crossproduct between V1 and V2
    out[1] =  v1[2]*v2[0] - v1[0]*v2[2];
    out[2] =  v1[0]*v2[1] - v1[1]*v2[0];

   normalize(out);
}

void drawtriangle(float *v1, float *v2, float *v3)
{

	GLfloat vecA[3], vecB[3], norm[3];
	for (int j = 0; j < 3; j++) {
		vecA[j] = v1[j] - v2[j];
		vecB[j] = v3[j] - v2[j];
	}
	normcrossprod(vecA, vecB, norm);
	// glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);
	glBegin(GL_TRIANGLES);
		glNormal3fv(norm);
		glVertex3fv(v1);
		glNormal3fv(norm);
		glVertex3fv(v2);
		glNormal3fv(norm);
		glVertex3fv(v3);
	glEnd();
}


void subdivide(float *v1, float *v2, float *v3, long depth)
{
  GLfloat v12[3], v23[3], v31[3];
  GLint i;
  if (depth == 0) {
    drawtriangle(v1, v2, v3);
    return;
  }
  for (i = 0; i < 3; i++) {
    v12[i] = (v1[i]+v2[i])/2.0;
    v23[i] = (v2[i]+v3[i])/2.0;
    v31[i] = (v3[i]+v1[i])/2.0;
  }
  normalize(v12);
  normalize(v23);
  normalize(v31);

  for (i = 0; i < 3; i++) {
		v12[i] = v12[i] * r;
		v23[i] = v23[i] * r;
		v31[i] = v31[i] * r;
	}

  subdivide(v1, v12, v31, depth-1);
  subdivide(v2, v23, v12, depth-1);
  subdivide(v3, v31, v23, depth-1);
  subdivide(v12, v23, v31, depth-1);
}

/*Draw object*/
void DrawIcosahedron(void)
{


    glBegin(GL_TRIANGLES);
    	for (int i = 0; i < 20; i++)
	    {
        subdivide(&vVertex[tindices[i][0]][0], &vVertex[tindices[i][1]][0], &vVertex[tindices[i][2]][0], 3);
   	  }
   glEnd();   // End Drawing Obj

}

/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.4f,0.8f,0.5f,1.0f); //   Set background (clear) color to black

   // glEnable(GL_LIGHTING);   //TODO: UNCOMMENT
   // glEnable(GL_LIGHT0);     //TODO: UNCOMMENT

   numActiveLights = MIN_VALUE(MAX_LIGHTS, 8);
   for (int i=0; i<numActiveLights;i++){
     initLight(i);
   }

   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, modelAmb);
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

   glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
   glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

   glNewList(ObjIcosahedron, GL_COMPILE);
         // DrawIcosahedron();                       //    TODO: Draw object,
         glutSolidSphere(1,20,20);
   glEndList();

   glNewList(DL_LIGHT_SPHERE, GL_COMPILE);
         glutSolidSphere(0.2,4,4);                       //    TODO: Draw object,
   glEndList();
}

/* Callback handler for window re-paint event */
void display()
{

   glClear (GL_COLOR_BUFFER_BIT); /*   clear screen */
   glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
   glLoadIdentity();              //   Reset model-view matrix


   gluLookAt(.0,0.0,zValue,  0.0,0.0,0.0,   0.0,1.0,0.0);      //  Define camera settings

    glPushMatrix();                               // TODO: Save State Matrix

    glDisable(GL_LIGHTING);
    for (int i=0; i< MAX_LIGHTS; i++){
      drawLight(&linfo[i]);
    }

    glEnable(GL_LIGHTING);
    for (int i=0;i<numActiveLights;i++){
      if (lightState[i]){
        int num = i;

        glLightfv(GL_LIGHT0 +i, GL_POSITION, linfo[num].xyz);
        glLightfv(GL_LIGHT0 +i, GL_DIFFUSE, linfo[num].rgb);
        glEnable(GL_LIGHT0 + i);
      } else {
        glDisable(GL_LIGHT0 +i);
      }
    }

    glPushMatrix();
      glColor3f(1.0,0.0,0.0);
      // glScalef(4,4,4);
      glCallList(ObjIcosahedron);

    glPopMatrix();



   // glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
   // glRotatef(angleTheta[1], 0.0, 1.0, 0.0);


   /* Draw object */

   // glColor3f(0.3,0.3,0.3);
   //  glCallList(ObjIcosahedron);                              // TODO: Call Instance  ObjIcosahedron
    glPopMatrix();
                              // TODO: Recover previous state matrix
    DrawAxis();
    glutSwapBuffers();

   // glFlush ();                   //  Render Object
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


   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 4, 500.0); //   Define the shape of your viewing volume using glFrustum function



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


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("Prac 5C");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
