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
GLuint vbo_triangle;

int init_resources()
{
	GLint compileOk = GL_FALSE, linkOk = GL_FALSE;
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const char * vsSource = 
#ifdef GL_ES_VERSION_2_0
		"#version 100 \n"
#else
		"#version 130 \n"
#endif
	//attribute vec3 coord2d; \n"
		"in vec4 position; \n"
		"out vec4 color; \n"
		"void main() { \n"
	// gl_Position = vec4(coord2d, 1.0); \n"
			"gl_Position = position; \n"
		" color = position; \n"
	
		"} \n";
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compileOk);
	if ( 0 == compileOk)
	{
		char log[300];
		GLsizeiptr logLen;

		glGetShaderInfoLog(vs,300,&logLen, log); 
		fprintf(stderr, vsSource);
		fprintf(stderr, log);

		return 0;
	}
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char * fsSource =
		"#version 130           \n"
		" in vec4 color; \n"
		"void main(void) {       \n "
		   "  gl_FragColor = color; \n"
	/*	" gl_FragColor[0] = gl_FragCoord.x/640.0; \n"
		"gl_FragColor[1] = gl_FragCoord.y/480.0; \n"
		"gl_FragColor[2] = 0.5; \n"
		"gl_FragColor[3] = floor(mod(gl_FragCoord.y, 2.0)); \n"*/
		"}";
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compileOk);
	if (!compileOk) {
		char log[300];
		GLsizeiptr logLen;

		glGetShaderInfoLog(fs,300,&logLen, log); 
		fprintf(stderr, fsSource);
		fprintf(stderr, log);
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
	/* -------------------------------

	-----------------------------------------------*/
	GLfloat triangle_vertices[] = {
		0.0,  0.8, 1.0, 1.0,
		-0.8, -0.8, 0.0, 1.0,
		0.8, -0.8, -1.0, 1.0,

	};
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), 
		triangle_vertices, GL_STATIC_DRAW);
	const char* attribute_name = "position";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	return 1;
}
void onDisplay()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glEnableVertexAttribArray(attribute_coord2d);

	/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
	glVertexAttribPointer(
		attribute_coord2d, // attribute
		4,                 // number of elements per vertex, here (x,y, z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0  // 
		);
	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(attribute_coord2d);

	glutSwapBuffers();
}
void free_resources()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_triangle);
}
int main (int argc, char * argv[])
{
	// GLUT-related initialization mechanizm
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA |GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH );
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






