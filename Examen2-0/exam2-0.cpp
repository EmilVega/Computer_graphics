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
#include <math.h>

#include "load-mesh.h"
// #include "vertices.h"
// #include "indices.h"

#define PI 3.14159265f
// #define MAX_POINTS 25

#define tea 1
#define plate 2
#define DL_LIGHT_SPHERE 3

static int numActiveLights;

#define MAX_LIGHTS (sizeof(linfo)/sizeof(linfo[0]))
#define MIN_VALUE(a,b) (((a)<(b))?(a):(b))
#define MAX_VALUE(a,b) (((a)>(b))?(a):(b))

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

// Define color Light //

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
  {{-50.0,100.0,-6.0,1.0}, magenta},
  {{50.0,100.0,3.0,1.0}, white},
  {{-2.0,100.0,100.0,1.0}, yellow},

};

int nSegments=20;


GLfloat angleTheta[] = {0.0,0.0,0.0};
 GLint axis = 2;

// Global variables
int windowWidth = 600;  // Initial Screen and viewport width
int windowHeight = 600; // Initil Screen and viewport height

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

void drawLight(LightInfo * info){
  glPushMatrix();
  glTranslatef(info->xyz[0], info->xyz[1], info->xyz[2]);
  glColor3fv(info->rgb);
  glScalef(50,50,50);
  glCallList(DL_LIGHT_SPHERE);
  glPopMatrix();
}

void initLight(int num){
  glLightf(GL_LIGHT0 + num, GL_CONSTANT_ATTENUATION, 0.0);
  glLightf(GL_LIGHT0 + num, GL_LINEAR_ATTENUATION, 0.0);
  glLightf(GL_LIGHT0 + num, GL_QUADRATIC_ATTENUATION, 0.1);
  glLightfv(GL_LIGHT0 + num, GL_SPECULAR, dim);
}


/*normalize a vector*/
void normalize(float v[3]) {
   GLfloat d = sqrt(v[0]*v[0] + v[1]*v[1]+v[2]*v[2]);            //TODO: compute the magnitude of the vector
   if (d == 0.0) {
      printf("\nError: zero length vector");
      v[0] =  0 ; v[1] = 0 ; v[2]= 0 ;
      // return;
   }else {
     v[0] =  v[0]/d ; v[1] = v[1]/d ; v[2]= v[2]/d ;    //TODO: normalize the vector
   }
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

// void drawBezierCurve()
// {
//     // glMap1f(GL_MAP1_VERTEX_3, 0.0, nSegments, 3, ncpoints, &points[0][0]);
//     // glEnable(GL_MAP1_VERTEX_3);
//
//     float u=0,v=0;
//     float vectU[4], vectV[4];
//     float matRes[nSegments+1][nSegments+1][3];
//
//     for(int fi=0; fi<32; fi++)
//     {
//
//       for (int a=0; a<=nSegments; a++)
//       {
//         for (int b=0; b<=nSegments; b++){
//           for(int c=0; c<3; c++){
//             matRes[a][b][c]=0;
//           }
//         }
//       }
//
//       for(int i=0; i<=nSegments; i++)
//       {
//
//           u=(float)i/(float)nSegments;
//           vectU[0]=pow(1-u,3);
//           vectU[1]=3*u*pow(1-u,2);
//           vectU[2]=3*u*u*(1-u);
//           vectU[3]=u*u*u;
//
//
//           for(int k=0; k<=nSegments; k++)
//           {
//             v=(float)k/(float)nSegments;
//             vectV[0]=pow(1-v,3);
//             vectV[1]=3*v*pow(1-v,2);
//             vectV[2]=3*v*v*(1-v);
//             vectV[3]=v*v*v;
//
//             float mul1[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
//
//             for (int c=0;c<4; c++){
//               for (int p=0;p<3;p++){
//                 mul1[c][p] = vectU[0]*vertices[tindices[fi][c]-1][p] +
//                              vectU[1]*vertices[tindices[fi][c+4]-1][p]  +
//                              vectU[2]*vertices[tindices[fi][c+8]-1][p]  +
//                              vectU[3]*vertices[tindices[fi][c+12]-1][p] ;
//               }
//             }
//
//             //float pT[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
//
//             float pT[3] = {0,0,0};
//             //for (int j = 0; j < 4; j++)
//             //{
//               for (int p=0;p<3;p++){
//                 pT[p]=mul1[0][p]*vectV[0]+
//                       mul1[1][p]*vectV[1]+
//                       mul1[2][p]*vectV[2]+
//                       mul1[3][p]*vectV[3];
//               }
//
//               matRes[i][k][0]=pT[0];
//               matRes[i][k][1]=pT[1];
//               matRes[i][k][2]=pT[2];
//             //}
//
//
//           }
//
//       }
//
//
//       GLfloat norm[3];
//
//       glBegin(GL_POLYGON);
//
//       for (int a=0; a<=nSegments; a++)
//       {
//         for (int b=0; b<=nSegments; b++){
//           norm[0]=matRes[a][b][0];
//           norm[1]=matRes[a][b][1];
//           norm[2]=matRes[a][b][2];
//           normalize(norm);
//           glNormal3fv(norm);
//           glVertex3f(matRes[a][b][0],matRes[a][b][1],matRes[a][b][2]);
//         }
//       }
//
//       for (int a=0; a<=nSegments; a++)
//       {
//         for (int b=0; b<=nSegments; b++){
//             norm[0]=matRes[b][a][0];
//             norm[1]=matRes[b][a][1];
//             norm[2]=matRes[b][a][2];
//             normalize(norm);
//             glNormal3fv(norm);
//             glVertex3f(matRes[b][a][0],matRes[b][a][1],matRes[b][a][2]);
//         }
//
//       }
//       glEnd();
//
//       //Mallado de la tetera
//
//       // for (int a=0; a<=nSegments; a++){
//       //     glBegin(GL_LINE_STRIP);
//       //   for (int b=0; b<=nSegments; b++){
//       //
//       //     glVertex3f(matRes[a][b][0],matRes[a][b][1],matRes[a][b][2]);
//       //   }
//       //   glEnd();
//       // }
//       //
//       // for (int a=0; a<=nSegments; a++){
//       //     glBegin(GL_LINE_STRIP);
//       //   for (int b=0; b<=nSegments; b++){
//       //
//       //     glVertex3f(matRes[b][a][0],matRes[b][a][1],matRes[b][a][2]);
//       //   }
//       //   glEnd();
//     }
//
// }

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

void DrawFloor(){
  float norm[3];
  glBegin(GL_POLYGON);
    glColor3f(1,0.,0);

    norm[0]=0; norm[1]=0; norm [2]=0;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(0,0,0);

    norm[0]=300; norm[1]=0; norm [2]=0;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(300,0,0);

    norm[0]=300; norm[1]=0; norm [2]=-300;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(300,0,-300);

    norm[0]=0; norm[1]=0; norm [2]=-300;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(0,0,-300);

    norm[0]=0; norm[1]=0; norm [2]=0;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(0,0,0);

    norm[0]=0; norm[1]=0; norm [2]=-300;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(0,0,-300);

    norm[0]=-300; norm[1]=0; norm [2]=-300;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(-300,0,-300);

    norm[0]=-300; norm[1]=0; norm [2]=0;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(-300,0,0);

    norm[0]=0; norm[1]=0; norm [2]=0;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(0,0,0);

    norm[0]=-300; norm[1]=0; norm [2]=0;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(-300,0,0);

    norm[0]=-300; norm[1]=0; norm [2]=300;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(-300,0,300);

    norm[0]=0; norm[1]=0; norm [2]=300;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(0,0,300);

    norm[0]=0; norm[1]=0; norm [2]=0;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(0,0,0);

    norm[0]=0; norm[1]=0; norm [2]=300;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(0,0,300);

    norm[0]=300; norm[1]=0; norm [2]=300;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(300,0,300);

    norm[0]=300; norm[1]=0; norm [2]=0;
    normalize(norm);
    glNormal3fv(norm);
    glVertex3f(300,0,0);
  glEnd();
}

/* Initialize OpenGL Graphics */
void initGL()
{
    glClearColor(0.4f,0.8f,0.5f,1.0f); // TODO: Set background (clear) color to black



    // glClear(GL_COLOR_BUFFER_BIT);

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



    // FRom c

    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    // glEnable(GL_AUTO_NORMAL);

    // glEnable(GL_LIGHTING);   //TODO: UNCOMMENT
    // glEnable(GL_LIGHT0);

    glNewList(tea, GL_COMPILE);
      // drawBezierCurve();
      load_mesh("teapot.obj");
      DrawObj();                      //    TODO: Draw object,
    glEndList();

    glNewList(plate, GL_COMPILE);
      DrawFloor();                       //    TODO: Draw object,
    glEndList();

    glNewList(DL_LIGHT_SPHERE, GL_COMPILE);
          glutSolidSphere(0.2,4,4);                       //    TODO: Draw object,
    glEndList();


}


// void DrawAxis(void)
// {
//     glBegin(GL_LINE_STRIP);
//     glColor3f(1.0f, 0.0f, 0.0f);
//     glVertex3f(0.0, 0.0, 0.0);
//     glVertex3f(250.0, 0.0, 0.0);
//     glColor3f(0.3f, 0.0f, 0.0f);
//     glVertex3f(-250.0, 0.0, 0.0);
//     glVertex3f(0.0, 0.0, 0.0);
//
//     glColor3f(0.0f, 1.0f, 0.0f);
//     glVertex3f(0.0, 250.0, 0.0);
//     glVertex3f(0.0, 0.0, 0.0);
//     glColor3f(0.0f, 0.3f, 0.0f);
//     glVertex3f(0.0, -250.0, 0.0);
//     glVertex3f(0.0, 0.0, 0.0);
//
//     glColor3f(0.0f, 0.0f, 1.0f);
//     glVertex3f(0.0, 0.0, 250.0);
//     glVertex3f(0.0, 0.0, 0.0);
//     glColor3f(0.0f, 0.0f, 0.3f);
//     glVertex3f(0.0, 0.0, -250.0);
//     glVertex3f(0.0, 0.0, 0.0);
//     glEnd();
// }


// void getMousePos3D(int x, int y, float outNewCoordinates[3])
// {
//     GLint viewport[4];
//     GLdouble modelview[16];
//     GLdouble projection[16];
//
//     GLfloat winX, winY, winZ;
//     GLdouble posX, posY, posZ;
//
//     glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
//     glGetDoublev(GL_PROJECTION_MATRIX, projection);
//     glGetIntegerv(GL_VIEWPORT, viewport);
//
//     winX = (float)x;
//     winY = (float)viewport[3] - (float)y - 1;
//     glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
//
//     gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
//
//     outNewCoordinates[0] = posX;
//     outNewCoordinates[1] = posY;
//     outNewCoordinates[2] = posZ;
// }

/* Callback handler for window re-paint event */
void display()
{
    GLfloat xL=0;
    GLfloat yL=200;
    GLfloat zL=100;

    // GLfloat light[3]={xL,yL,zL};
    GLfloat m[16] = { 1,0,0,0,0,1,0,-1/yL,0,0,1,0,0,0,0,0 };


    glClear(GL_COLOR_BUFFER_BIT); /* TODO: clear screen */
    glMatrixMode(GL_MODELVIEW); // TODO: To operate on the model-view matrix
    glLoadIdentity();           // TODO: Reset model-view matrix

    gluLookAt(0.0,6,200,  0.0,0.0,0.0,   0.0,1.0,0.0);

    glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
    glRotatef(angleTheta[1], 0.0, 1.0, 0.0);

    glPushMatrix();
    // Luces


    // glPushMatrix();
//
    // DrawAxis();


    // glPushMatrix();

    glDisable(GL_LIGHTING);
    glCallList(plate);
    // glDisable(GL_LIGHTING);

    glPopMatrix();
    glPushMatrix();

    // Grafica de la luz
    // glBegin(GL_POLYGON);
    //  glColor3f(1,1,1);
    //  glVertex3f(-305,405,30);
    //  glVertex3f(-295,405,30);
    //  glVertex3f(-295,395,30);
    //  glVertex3f(-304,395,30);
    // glEnd();

    glDisable(GL_LIGHTING);
    glTranslatef(xL,yL,zL);
    glMultMatrixf(m);
    glTranslatef(-xL,-yL,-zL);
    glColor3f(0.5,0.5,0.5);
    glScalef(150,150,150);
    // glRotatef(180,1.0,0.0,0.0);
    glCallList(tea);

    glPopMatrix();
    glPushMatrix();

    // glEnable(GL_LIGHTING);   //TODO: UNCOMMENT
    // glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColor3f(0.0, 0.0, 1.0);
    // glRotatef(-90,1.0,0.0,0.0);
    glScalef(150,150,150);

    glCallList(tea);


    glPopMatrix();

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




    glutSwapBuffers();

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
 if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
 if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 1;
 //if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 2;
 angleTheta[axis] -= 2.0;
 //if( angleTheta[axis] > 360.0 ) angleTheta[axis] -= 360.0;
 display();
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
