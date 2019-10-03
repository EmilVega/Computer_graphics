/****************************************************************************/
/* Prac. 10A flat mesh and Shader                    */
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


#define PI 3.14159265f
#define ObjMesh 1

// Global variables
int windowWidth  = 500;     // Initial Screen and viewport width 
int windowHeight = 500;     // Initil Screen and viewport height

GLfloat zValue=250;
//GLfloat zValue=7;
 GLfloat xRotated, yRotated, zRotated;
 static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 2;
int factorSize = 20;

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;


//sqrt(X² + Y²) = r
#define X 20
#define Z 40
GLfloat radius=sqrt(X*X + Z*Z);
int subdiv = 4;	
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

GLfloat  norm[3];   
//************SHADER VARIABLES*************
#define N 200

//const GLdouble nearVal     = 1.0; /* near distance */
//const GLdouble farVal      = 20.0; /* far distance */
const GLfloat  lightPos[4] = {-50.0f, 100.0f, 60.0f, 1.0f}; /* light position */
const float meshColor[]     = {1.0f, 0.0f, 0.0f, 1.0f}; 
const float meshSpecular[]  = {0.8f, 0.8f, 0.8f, 1.0f};
const float meshShininess[] = {80.0f};

GLuint         program     = 0; /* program object id */
GLint          timeParam;


GLchar *ebuffer; /* buffer for error messages */
GLsizei elength; /* length of error message */

GLfloat data[N][N]; /* array of data heights */
//************END SHADER VARIABLES**********

//************SHADER FUNCTIONS*************
/* shader reader */
/* creates null terminated string from file */

static char* readShaderSource(const char* shaderFile)
{
    FILE* fp = fopen(shaderFile, "rb");
	
    char* buf;
    long size;

    if(fp==NULL) return NULL;
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    buf = (char*) malloc((size+1) * sizeof(char)); 
    fread(buf, 1, size, fp);
	buf[size] = '\0';
    fclose(fp);
    return buf;
}

/* GLSL initialization */

static void initShader(const GLchar* vShaderFile, const GLchar* fShaderFile)
{
    glewExperimental=GL_TRUE;
    GLenum err = glewInit();
   printf ("%d",err);
if (err != GLEW_OK)
  { //printf ("OK%s", s);
  exit(1); // or handle the error in a nicer way
}
if (!GLEW_VERSION_2_1)  // check that the machine supports the 2.1 API.
  exit(1); // or handle the error in a nicer way


    GLint status; 
    GLchar *vSource, *fSource;
    GLuint vShader, fShader;

    /* read shader files */

    vSource = readShaderSource(vShaderFile);
    if(vSource==NULL)
    {
        printf( "Failed to read vertex shader\n");
        exit(EXIT_FAILURE);
    }

    fSource = readShaderSource(fShaderFile);
    if(fSource==NULL)
    {
        printf("Failed to read fragment shader");
       exit(EXIT_FAILURE);
    }

    /* create program and shader objects */

    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();

    /* attach shaders to the program object */

    glAttachShader(program, vShader); 
    glAttachShader(program, fShader);

    /* read shaders */

    glShaderSource(vShader, 1, (const GLchar**) &vSource, NULL);
    glShaderSource(fShader, 1, (const GLchar**) &fSource, NULL);

    /* compile vertex shader shader */

    glCompileShader(vShader);

    /* error check */
    
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    if(status==GL_FALSE)
    {
       printf("Failed to compile the vertex shader.\n");
       glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &elength);
       ebuffer = (GLchar *) malloc(elength*sizeof(char));
       glGetShaderInfoLog(vShader, elength, NULL, ebuffer);
       printf("%s\n", ebuffer);
      exit(EXIT_FAILURE);
    }

    /* compile fragment  shader shader */

    glCompileShader(fShader);

    /* error check */

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
    if(status==GL_FALSE)
    {
       printf("Failed to compile the fragment shader.\n");
       glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &elength);
       ebuffer = (GLchar *) malloc(elength*sizeof(char));
       glGetShaderInfoLog(fShader, elength, NULL, ebuffer);
       printf("%s\n", ebuffer); 
       exit(EXIT_FAILURE);
    }

    /* link  and error check */

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status==GL_FALSE)
    {
       printf("Failed to link program object.\n");
       glGetProgramiv(program, GL_INFO_LOG_LENGTH, &elength);
       ebuffer = (GLchar *) malloc(elength*sizeof(char));
       glGetProgramInfoLog(program, elength, &elength, ebuffer);
       printf("%s\n", ebuffer);
       exit(EXIT_FAILURE);
    }
    
    /* use program object */

    glUseProgram(program);

    /* set up uniform parameter */

    timeParam = glGetUniformLocation(program, "time");
    printf("OK");
}


void drawLight( )
{
  glPushMatrix();
  glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
  glColor3f(meshColor[0],meshColor[1],meshColor[2]);
  glutSolidSphere(10, 4, 4);
  glPopMatrix();
}
//************ END SHADER FUNCTIONS*************

/*normalize a vector*/
void normalize(float v[3]) {    
   GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); 
   if (d == 0.0) {
      printf("\nErrot: zero length vector");    
      return;
   }
   v[0] /= d; v[1] /= d; v[2] /= d; 
   norm[0]=v[0];   
   norm[1]=v[1];   
   norm[2]=v[2];   
}

//v1[], v2[] are two vectors
//out[] holds the crossproduct v1 x v2 
void normcrossprod(float v1[3], float v2[3], float out[3]) 
{ 
   GLint i, j; 
   GLfloat length;

   /*out[0] = v1[1]*v2[2] - v1[2]*v2[1]; 
   out[1] = v1[2]*v2[0] - v1[0]*v2[2]; 
   out[2] = v1[0]*v2[1] - v1[1]*v2[0]; */
   out[0] = v1[1]*v2[2] - v1[2]*v2[1]; 
   out[1] = - (v1[0]*v2[2] - v1[2]*v2[0]); 
   out[2] = v1[0]*v2[1] - v1[1]*v2[0]; 

   normalize(out); 
}

void normalizeVertexR(float v[3]) {    
   GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); 
   if (d == 0.0) {
      printf("\nErrot: zero length vector");    
      return;
   }
   v[0] = (v[0]/d)*radius; v[1] = (v[1] /d)*radius; v[2] =(v[2]/d)*radius; 
}

/*
  Use subdivision to create 80-sided spherical approximation
*/
void drawtriangle(float *v1, float *v2, float *v3) 
{ 
  GLfloat vecA[3], vecB[3], norm[3];   
  vecA[0]=v1[0] - v2[0];
  vecA[1]=v1[1] - v2[1];
  vecA[2]=v1[2] - v2[2];

  vecB[0]=v2[0] - v3[0];
  vecB[1]=v2[1] - v3[1];
  vecB[2]=v2[2] - v3[2];
   
  normcrossprod(vecA, vecB, norm); 
  //glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
   glBegin(GL_TRIANGLES); 
      //glNormal3fv(norm); 
      glNormal3fv(v1); 
      glVertex3fv(v1);    

      //glNormal3fv(norm);
      glNormal3fv(v2);  
      glVertex3fv(v2);    

      //glNormal3fv(norm); 
      glNormal3fv(v3); 
      glVertex3fv(v3);    
   glEnd(); 
}

void subdivide(float *v1, float *v2, float *v3, int depth) 
{ 
   GLfloat v12[3], v23[3], v31[3];    
   GLint i;

   if (depth == 0) 
   {
	drawtriangle(v1,v2,v3);
        return;
   }

   for (i = 0; i < 3; i++) { 
      v12[i] = ( v1[i]+v2[i] ) / 2.0; 
      v23[i] = ( v2[i]+v3[i] ) / 2.0;     
      v31[i] = ( v3[i]+v1[i] ) / 2.0;    
   } 
   normalizeVertexR(v12);
   normalizeVertexR(v23);
   normalizeVertexR(v31);
   subdivide(v1, v12, v31, depth-1);    
   subdivide(v2, v23, v12, depth-1);    
   subdivide(v3, v31, v23, depth-1);    
   subdivide(v12, v23, v31, depth-1); 
}

void DrawMeshObj(void)
{   
    initShader("prac10Bvertexmesh.gls.txt", "prac10Bfragmentmesh.glsl.txt");
    //initShader("fPhong.glsl.txt", "prac10Bfragmentmesh.glsl.txt");
    glutSolidSphere(80, 20, 15);  
}

void DrawMeshObj2(void)
{   
    initShader("prac10Bvertexmesh.gls.txt", "prac10Bfragmentmesh.glsl.txt");
    //glColor3f((double)(rand()%256),(double)(rand()%256),(double)(rand()%256));  //   Set the color of the object
    //glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);

    	for (int i = 0; i < 20; i++) 
	  subdivide(&vVertex[tindices[i][0]][0], &vVertex[tindices[i][1]][0], &vVertex[tindices[i][2]][0], subdiv);
          
              
}

/*
	Your display function should look roughly like the following.
*/
void DrawMeshObj1()
{
    initShader("prac10Bvertexmesh.gls.txt", "prac10Bfragmentmesh.glsl.txt");
        /*
    int i,j; 
    for(i=0; i<N; i++) for(j=0; j<N;j++)
    {
       glColor3f(1.0, 1.0, 1.0);
       glBegin(GL_POLYGON);
          glVertex3f((float)i*100/N, data[i][j], (float)j*100/N);
          glVertex3f((float)i*100/N, data[i][j], (float)(j*100+1)/N);
          glVertex3f((float)(i*100+1)/N, data[i][j], (float)(j*100+1)/N);
          glVertex3f((float)(i*100+1)/N, data[i][j], (float)(j*100)/N);
       glEnd();  
       glColor3f(1.0, 0.0, 0.0);
       glBegin(GL_LINE_LOOP);
          glVertex3f((float)i*100/N, data[i][j], (float)j*100/N);
          glVertex3f((float)i*100/N, data[i][j], (float)(j*100+1)/N);
          glVertex3f((float)(i*100+1)/N, data[i][j], (float)(j*100+1)/N);
          glVertex3f((float)(i*100+1)/N, data[i][j], (float)(j*100)/N);
       glEnd();  
    }*/
    glEnable(GL_NORMALIZE);
    glBegin(GL_TRIANGLES);   
    	for (int i = 0; i < 20; i++) 
	{   
	  /* color information here */
	  //glColor3f((double)(rand()%256),(double)(rand()%256),(double)(rand()%256));  //   Set the color of the object
	  //glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
	  //Normal information
	  /*
	  Note: difference between two points is a vector
	  i.e. 	A = P0 - P1
	 	B = P2 -  p1
		A x B is normal to the plane passing through points P0, P1, P2,
		and P0, P1, P2 appear counter-clockwise
	  ( see notes above )
	 */
	  GLfloat vecA[3], vecB[3];// norm[3];    
	  for (int j = 0; j < 3; j++) 
	  {    
	     vecA[j] = vVertex[tindices[i][0]][j] - vVertex[tindices[i][1]][j];    
	     vecB[j] = vVertex[tindices[i][2]][j] - vVertex[tindices[i][1]][j];    
	   }
	   normcrossprod(vecA, vecB, norm); 
           

	   //vertex information
           //normalize(&vVertex[tindices[i][0]][0]);
	   glNormal3fv(norm);     	   
	   glVertex3fv( &vVertex[tindices[i][0]][0]);

	   //normalize(&vVertex[tindices[i][1]][0]); 
           glNormal3fv(norm); 
	   glVertex3fv(&vVertex[tindices[i][1]][0]); 

	   //normalize(&vVertex[tindices[i][2]][0]);
           glNormal3fv(norm);
	   glVertex3fv(&vVertex[tindices[i][2]][0]); 
           
   	}
   
    glEnd();   // End Drawing Obj     
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

    

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, meshColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, meshSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, meshShininess);


   glClearColor(1.0, 1.0, 1.0, 1.0); //   Set background (clear) color to black
 
 
   glNewList(ObjMesh, GL_COMPILE);	
      glColor3f(0.0f, 1.0f, 0.0f);
      DrawMeshObj();       //    Draw a object,  
   glEndList();
}

/* Callback handler for window re-paint event */
void display() 
{

   //glUniform1f(timeParam, glutGet(GLUT_ELAPSED_TIME));
   glUniform1f(timeParam,  (float)500.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //mesh();
    //glutSwapBuffers();
   
   //glClear (GL_COLOR_BUFFER_BIT); /*   clear screen */
   glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
   glLoadIdentity();              //   Reset model-view matrix
   
   
   gluLookAt(.0,0.0,zValue,  0.0,0.0,0.0,   0.0,1.0,0.0);      //  Define camera settings
    glDisable(GL_LIGHTING); 
    glDisable(GL_LIGHT0);
    //glDisable(GL_DEPTH_TEST);

   drawLight();
     
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0);
    //glEnable(GL_DEPTH_TEST);


   
   glPushMatrix();
    glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
    glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
   
   
   /* Draw obj */ 
   //glTranslatef(0,-1,0);
   //glScalef(200,200,20);
   glCallList(ObjMesh);  
   //DrawMeshObj();
   glPopMatrix();
 
  

   
   //drawLight();
   DrawAxis();
   //glutSolidSphere(200, 4, 4);
   //glFlush();
   glutSwapBuffers ();                   //  Render Object
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

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   for(int i=0;i<N;i++) for(int j=0;j<N;j++) data[i][j]=0.0;

   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("Prac 6A");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
