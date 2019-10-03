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

#define PI 3.14159265f

#define ObjIcosahedron 1
 
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

//sqrt(X² + Y²) = r
#define X 50
#define Z 80
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

/*Draw object*/
void DrawIcosahedron(void)
{   
    //glColor3f((double)(rand()%256),(double)(rand()%256),(double)(rand()%256));  //   Set the color of the object
    //glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
    glBegin(GL_TRIANGLES);    
    	for (int i = 0; i < 20; i++) 
	{    
	  /* color information here */
	  //glColor3f((double)(rand()%256),(double)(rand()%256),(double)(rand()%256));  //   Set the color of the object
	  glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
	  //Normal information
	  /*
	  Note: difference between two points is a vector
	  i.e. 	A = P0 - P1
	 	B = P2 -  p1
		A x B is normal to the plane passing through points P0, P1, P2,
		and P0, P1, P2 appear counter-clockwise
	  ( see notes above )
	 */
	  GLfloat vecA[3], vecB[3], norm[3];    
	  for (int j = 0; j < 3; j++) 
	  {    
	     vecA[j] = vVertex[tindices[i][0]][j] - vVertex[tindices[i][1]][j];    
	     vecB[j] = vVertex[tindices[i][2]][j] - vVertex[tindices[i][1]][j];    
	   }
	   normcrossprod(vecA, vecB, norm); 
            

	   //vertex information
	   glNormal3fv(norm);
	   glVertex3fv( &vVertex[tindices[i][0]][0]);
           glNormal3fv(norm); 
	   glVertex3fv(&vVertex[tindices[i][1]][0]); 
           glNormal3fv(norm);
	   glVertex3fv(&vVertex[tindices[i][2]][0]); 
   	}
   glEnd();   // End Drawing Obj
              
}

/* Initialize OpenGL Graphics */
void initGL() 
{
   glClearColor(0.0, 0.0, 0.0, 1.0); //   Set background (clear) color to black

   glEnable(GL_LIGHTING);   //TODO: UNCOMMENT
   glEnable(GL_LIGHT0);     //TODO: UNCOMMENT
   glNewList(ObjIcosahedron, GL_COMPILE);	
      DrawIcosahedron();        //    Draw a object,  
   glEndList();
}

/* Callback handler for window re-paint event */
void display() 
{
   
   glClear (GL_COLOR_BUFFER_BIT); /*   clear screen */
   glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
   glLoadIdentity();              //   Reset model-view matrix
   
   
   gluLookAt(.0,0.0,zValue,  0.0,0.0,0.0,   0.0,1.0,0.0);      //  Define camera settings
   glPushMatrix();
    glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
    glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
   
   
   /* Draw obj */
   
   glCallList(ObjIcosahedron);        //   Draw obj   
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


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("Prac 5A");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
