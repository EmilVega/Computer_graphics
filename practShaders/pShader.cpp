// ***************** SHADER VARIABLES *******************


const GLfloat lightPos[4] = {-50.0f, 100.0f, 60.0f, 1.0f}; /* light position */
const float meshColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
const float meshSpecular[]= {0.8f, 0.8f, 0.8f, 1.0f};
const float meshShininess[] = {80.0f};

GLuint  program = 0; /* program object id */
GLuint  timeParam;

GLchar *ebuffer; /* buffer for error messages */
GLsizei elenght; /* lenght of error message */

//******************** END SHADER VARIABLES ************


//******************* SHADER FUNCTIONS *****
/* shader reader */
/* creates null terminated string from file */

static char* readShaderSource(const char* shaderFile)
{
  FILE* fp = fopen(shaderFile, "rb");

  char* buf;
  long size;

  if (fp==NULL) return NULL;
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
  printf ("%d", err);
  if (err != GLEW_OK)
  {
    //printf ("OK%s", s);
    exit(1); // or handle the error in a nicer way
  }

  GLint status;
  GLchar *vSource, *fSource;
  GLuint vShader, fShader;

  /* read shader files */

  vSource = readShaderSource(vShaderFile);
  if(vSource==NULL)
  {
    printf("Failed to read vertexshader \n");
    exit(EXIT_FAILURE);
  }

  fSource = readShaderSource(fShaderFile);
}
