/* use the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
// use glew instead od gl.h to get all GL prototype
#include "GL\glew.h"
// use the freegult library for base windowing setup
#include "GL\freeglut.h"

// GLOBALS
GLuint program;
GLint attribute_coord2d;

int init_resources()
{
	GLint compileOk = GL_FALSE, linkOk = GL_FALSE;
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const char * vsSource = 
#ifdef GL_ES_VERSION_2_0
		"#version 100 \n"
#else
		"#version 120 \n"
#endif
		"attribute vec2 coord2d; \n"
		"void main() { \n"
		" gl_Position = vec4(coord2d, 0.0, 1.0); \n"
		"}";
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compileOk);
	if ( 0 == compileOk)
	{
		fprintf(stderr,"Error in vertex shader \n");
		return 0;
	}
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char * fsSource =
    "#version 120           \n"
    "void main(void) {        "
    "  gl_FragColor[0] = 0.0; "
    "  gl_FragColor[1] = 0.0; "
    "  gl_FragColor[2] = 1.0; "
    "}";
  glShaderSource(fs, 1, &fsSource, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &compileOk);
  if (!compileOk) {
    fprintf(stderr, "Error in fragment shader\n");
    return 0;
  }
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &linkOk);
  if (!linkOk) {
    fprintf(stderr, "glLinkProgram:");
    return 0;
  }
  const char* attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

	return 1;
}
void onDisplay()
{
  /* Clear the background as white */
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
 
  glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord2d);
  GLfloat triangle_vertices[] = {
     0.0,  0.8,
    -0.8, -0.8,
     0.8, -0.8,
  };
  /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
  glVertexAttribPointer(
    attribute_coord2d, // attribute
    2,                 // number of elements per vertex, here (x,y)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    triangle_vertices  // pointer to the C array
  );
 
  /* Push each element in buffer_vertices to the vertex shader */
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(attribute_coord2d);
 
  /* Display the result */
  glutSwapBuffers();
}
void free_resources()
{
	 glDeleteProgram(program);
}
int main (int argc, char * argv[])
{
	// GLUT-related initialization mechanizm
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(640, 480);
	glutCreateWindow("Mirek's first window ");
	//extension wrangler ninit
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		fprintf(stderr, "Error: %s \n", glewGetErrorString(glewStatus));
		return EXIT_FAILURE;
	}
	if (init_resources() == 1)
	{
		glutDisplayFunc(onDisplay);
		glutMainLoop();
	}
	free_resources();
	return EXIT_SUCCESS;
}






