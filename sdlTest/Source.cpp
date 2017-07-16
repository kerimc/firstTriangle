/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/
#include "include\AntTweakBar.h"
//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#include "include\sdl\SDL.h"

#include "include\sdl\SDL_opengl.h"

#include "include\glm\glm.hpp"
#include "include\glm\gtc\matrix_transform.hpp"
#include "include\glm\gtc\type_ptr.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifdef _WIN32
#   include <windows.h> // required by gl.h
#endif
#include <GL/gl.h>
#include <GL/glu.h>

//In this example, we draw a simple rotating square using the OpenGL core profile
// (which requires much more code than with the compatibility profile).
// A tweak bar is created to allow the user to change the color of the 
// rectangle and see its rotation.

// Part of OpenGL core interface is not directly accessible from the common
// OpenGL header and library (at least on windows) so we have to retrieve the
// core functions using glGetProcAddress. These functions are prefixed by
// underscore to avoid possible confict if a modified gl.h has been installed.

#ifdef _WIN32
#   define glGetProcAddress wglGetProcAddress
#else
#   define GLXglXEXT_LEGACY
#   include <GL/glx.h>
#   define glGetProcAddress glXGetProcAddressARB
#endif
#ifndef APIENTRY
#   define APIENTRY
#endif
typedef GLuint(APIENTRY *PFNGLCreateShader)(GLenum type);
typedef void (APIENTRY *PFNGLDeleteShader)(GLuint shader);
typedef void (APIENTRY *PFNGLShaderSource)(GLuint shader, GLsizei count, const char* *str, const GLint *length);
typedef void (APIENTRY *PFNGLCompileShader)(GLuint shader);
typedef void (APIENTRY *PFNGLAttachShader)(GLuint program, GLuint shader);
typedef GLuint(APIENTRY *PFNGLCreateProgram)(void);
typedef void (APIENTRY *PFNGLLinkProgram)(GLuint program);
typedef void (APIENTRY *PFNGLUseProgram)(GLuint program);
typedef void (APIENTRY *PFNGLDeleteProgram)(GLuint program);
typedef void (APIENTRY *PFNGLGenBuffers)(GLsizei n, GLuint *buffers);
typedef void (APIENTRY *PFNGLBindBuffer)(GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY *PFNGLEnableVertexAttribArray)(GLuint index);
typedef void (APIENTRY *PFNGLGenVertexArrays)(GLsizei n, GLuint *arrays);
typedef void (APIENTRY *PFNGLBindVertexArray)(GLuint array);
typedef void (APIENTRY *PFNGLDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
typedef GLint(APIENTRY *PFNGLGetAttribLocation)(GLuint program, const char *name);
typedef GLint(APIENTRY *PFNGLGetUniformLocation)(GLuint program, const char *name);
typedef void (APIENTRY *PFNGLUniform1f)(GLint location, GLfloat v0);
typedef void (APIENTRY *PFNGLUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY *PFNGLUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY *PFNGLBufferData)(GLenum target, ptrdiff_t size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY *PFNGLDeleteBuffers)(GLsizei n, const GLuint *buffers);


//GLAPI void APIENTRY glGetShaderiv (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
//typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGetProgramiv) (GLuint program, GLenum pname, GLint *params);
// typedef GLboolean (APIENTRYP PFNGLISSHADERPROC) (GLuint shader);
typedef GLboolean (APIENTRYP PFNGLISSHADERPROC) (GLuint shader);
// typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
// typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
// typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
//typedef GLboolean (APIENTRYP PFNGLISPROGRAMPROC) (GLuint program);
typedef GLboolean(APIENTRYP PFNGLISPROGRAMPROC) (GLuint program);
// typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

typedef void (APIENTRYP PFNGLCLEARCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
PFNGLCreateShader glCreateShader;
PFNGLDeleteShader glDeleteShader;
PFNGLShaderSource glShaderSource;
PFNGLCompileShader glCompileShader;
PFNGLAttachShader glAttachShader;
PFNGLCreateProgram glCreateProgram;
PFNGLLinkProgram glLinkProgram;
PFNGLUseProgram glUseProgram;
PFNGLDeleteProgram glDeleteProgram;
PFNGLGenBuffers glGenBuffers;
PFNGLBindBuffer glBindBuffer;
PFNGLVertexAttribPointer glVertexAttribPointer;
PFNGLEnableVertexAttribArray glEnableVertexAttribArray;
PFNGLGenVertexArrays glGenVertexArrays;
PFNGLBindVertexArray glBindVertexArray;
PFNGLDeleteVertexArrays glDeleteVertexArrays;
PFNGLGetAttribLocation glGetAttribLocation;
PFNGLGetUniformLocation glGetUniformLocation;
PFNGLUniform1f glUniform1f;
PFNGLUniform3f glUniform3f;
PFNGLUniform4f glUniform4f;
PFNGLBufferData glBufferData;
PFNGLDeleteBuffers glDeleteBuffers;
PFNGLGetShaderiv glGetShaderiv;
PFNGLGetProgramiv glGetProgramiv;
PFNGLISSHADERPROC glIsShader;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLISPROGRAMPROC glIsProgram;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;


#ifndef GL_ARRAY_BUFFER
#   define GL_ARRAY_BUFFER      0x8892
#endif
#ifndef GL_STATIC_DRAW
#   define GL_STATIC_DRAW       0x88E4
#endif
#ifndef GL_VERTEX_SHADER
#   define GL_VERTEX_SHADER     0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#   define GL_FRAGMENT_SHADER   0x8B30
#endif

int LoadGLCoreFunctions()
{
	glCreateShader = (PFNGLCreateShader)glGetProcAddress("glCreateShader");
	glDeleteShader = (PFNGLDeleteShader)glGetProcAddress("glDeleteShader");
	glShaderSource = (PFNGLShaderSource)glGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCompileShader)glGetProcAddress("glCompileShader");
	glAttachShader = (PFNGLAttachShader)glGetProcAddress("glAttachShader");
	glCreateProgram = (PFNGLCreateProgram)glGetProcAddress("glCreateProgram");
	glLinkProgram = (PFNGLLinkProgram)glGetProcAddress("glLinkProgram");
	glUseProgram = (PFNGLUseProgram)glGetProcAddress("glUseProgram");
	glDeleteProgram = (PFNGLDeleteProgram)glGetProcAddress("glDeleteProgram");
	glGenBuffers = (PFNGLGenBuffers)glGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBindBuffer)glGetProcAddress("glBindBuffer");
	glVertexAttribPointer = (PFNGLVertexAttribPointer)glGetProcAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (PFNGLEnableVertexAttribArray)glGetProcAddress("glEnableVertexAttribArray");
	glGenVertexArrays = (PFNGLGenVertexArrays)glGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBindVertexArray)glGetProcAddress("glBindVertexArray");
	glDeleteVertexArrays = (PFNGLDeleteVertexArrays)glGetProcAddress("glDeleteVertexArrays");
	glGetAttribLocation = (PFNGLGetAttribLocation)glGetProcAddress("glGetAttribLocation");
	glGetUniformLocation = (PFNGLGetUniformLocation)glGetProcAddress("glGetUniformLocation");
	glUniform1f = (PFNGLUniform1f)glGetProcAddress("glUniform1f");
	glUniform3f = (PFNGLUniform3f)glGetProcAddress("glUniform3f");
	glBufferData = (PFNGLBufferData)glGetProcAddress("glBufferData");
	glDeleteBuffers = (PFNGLDeleteBuffers)glGetProcAddress("glDeleteBuffers");

	glGetShaderiv = (PFNGLGetShaderiv)glGetProcAddress("glGetShaderiv");
	glGetProgramiv = (PFNGLGetProgramiv)glGetProcAddress("glGetProgramiv");
	glIsShader = (PFNGLISSHADERPROC)glGetProcAddress("glIsShader");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glGetProcAddress("glGetShaderInfoLog");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glGetProcAddress("glUniformMatrix4fv");
	glDisableVertexAttribArray =(PFNGLDISABLEVERTEXATTRIBARRAYPROC)glGetProcAddress("glDisableVertexAttribArray");
	glIsProgram = (PFNGLISPROGRAMPROC)glGetProcAddress("glIsProgram");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glGetProcAddress("glGetProgramInfoLog");


	if (glCreateShader == NULL || glDeleteShader == NULL || glShaderSource == NULL || glCompileShader == NULL
		|| glAttachShader == NULL || glCreateProgram == NULL || glLinkProgram == NULL || glUseProgram == NULL
		|| glDeleteProgram == NULL || glGenBuffers == NULL || glBindBuffer == NULL || glVertexAttribPointer == NULL
		|| glEnableVertexAttribArray == NULL || glGenVertexArrays == NULL || glBindVertexArray == NULL
		|| glDeleteVertexArrays == NULL || glGetAttribLocation == NULL || glGetUniformLocation == NULL
		|| glUniform1f == NULL || glUniform3f == NULL || glBufferData == NULL || glDeleteBuffers == NULL)
		return 0;
	else
		return 1;
}



//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes rendering program and clear color
bool initGL();

//Input handler
void handleKeys(unsigned char key, int x, int y);

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

//Shader loading utility programs
void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderQuad = true;
float zoom = 0.0f;
float rot1 = 0.0f;

//
float angle = 0, quat[4];
float color[] = { 0.8f, 1.0f, 0.2f };

//Graphics program
GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLint gVertexColorLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;
GLuint gCBO = 0;
TwBar *CreateTweakBar()
{
	TwBar *bar;

	// Create a tweak bar
	bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with SDL and OpenGL Core Profile.\n' "); // Message added to the help bar.

																													 // Add variables
	TwAddVarRW(bar, "Rotation", TW_TYPE_QUAT4F, &quat, "opened=true help='Rectangle rotation axisz=-z' ");
	TwAddVarRW(bar, "Color", TW_TYPE_COLOR3F, &color, "opened=true help='Rectangle color' ");
	TwAddVarRW(bar, "Zoom", TW_TYPE_FLOAT, &zoom, "help='sssssssssssss'");

	return bar;
}
bool init()
{

	LoadGLCoreFunctions();

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{

	
		//Use OpenGL 4.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize GL Functions
				int loadError = 0;
				loadError = LoadGLCoreFunctions();
				if (loadError == 1)
				{
					printf("Error initializing GL function pointers! \n");
				}

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				if (!TwInit(TW_OPENGL_CORE, NULL)) {
					fprintf(stderr, "AntTweakBar initialization failed: %s\n", TwGetLastError());
					SDL_Quit();
					exit(1);
				}
				// Tell the window size to AntTweakBar
				TwWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

				//Initialize OpenGL
				if (!initGL())
				{
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}
	

	return success;
}



bool initGL()
{
	//Success flag
	bool success = true;

	//Generate program
	gProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source
	const GLchar* vertexShaderSource[] =
	{
		"#version 140\n"
		"in vec4 LVertexPos;\n"
		"in vec4 LVertexColor;\n"
		"out vec4 vcolor;\n"
		"uniform mat4 mvp;"
		"void main() { \n"
		"gl_Position = mvp *  LVertexPos;\n"
		"vcolor = LVertexColor; \n"
	"} \n"
	};

	//Set vertex source
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	//Compile vertex source
	glCompileShader(vertexShader);

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		success = false;
	}
	else
	{
		//Attach vertex shader to program
		glAttachShader(gProgramID, vertexShader);


		//Create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Get fragment source
		const GLchar* fragmentShaderSource[] =
		{
			"#version 140\n"
			" in vec4 vcolor; \n"
			"out vec4 LFragment;\n"
			"uniform vec3 sumCo;"
			"void main() {\n"
			//"LFragment = vec4( 0.0, 1.0, 0.0, 1.0 );\n"
			"LFragment = vcolor + vec4(sumCo.r, sumCo.g , sumCo.b, 1.0);\n"
			"}\n"
		};

		//Set fragment source
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		//Compile fragment source
		glCompileShader(fragmentShader);

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
			printShaderLog(fragmentShader);
			success = false;
		}
		else
		{
			//Attach fragment shader to program
			glAttachShader(gProgramID, fragmentShader);


			//Link program
			glLinkProgram(gProgramID);

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", gProgramID);
				printProgramLog(gProgramID);
				success = false;
			}
			else
			{
				//Get vertex attribute location
				gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos");
				if (gVertexPos2DLocation == -1)
				{
					printf("LVertexPos is not a valid glsl program variable!\n");
					success = false;
				}
				gVertexColorLocation = glGetAttribLocation(gProgramID, "LVertexColor");
				if (gVertexColorLocation == -1)
				{
					printf("LVertexColor is not a valid glsl program variable!\n");
					success = false;
				}
				else
		
				{
					//Initialize clear color
					glClearColor(0.f, 0.f, 0.f, 1.f);

					//VBO data
					GLfloat vertexData[] =
					{
						-0.5f, -0.5f, 0.5f, 1.0f,
						0.5f, -0.5f, 0.5f, 1.0f,
						0.5f,  0.5f, 0.5f, 1.0f,
						-0.5f,  0.5f, 0.5f, 1.0f,

						-0.5f, -0.5f, -0.5f, 1.0f,
						0.5f, -0.5f, -0.5f, 1.0f,
						0.5f,  0.5f, -0.5f, 1.0f,
						-0.5f,  0.5f, -0.5f, 1.0f
					};

					//IBO data
					GLuint indexData[] = { 0, 1, 2, 3,
										    3, 7, 4,0,
											0, 4, 5, 1,
											6, 5 ,2, 1};

					//color vertex
					GLfloat vertexColorData[] =
					{
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 1.0f,

						1.0f, 0.0f, 0.0f, 1.0f,
						1.0f, 0.0f, 0.0f, 1.0f,
						1.0f, 0.0f, 0.0f, 1.0f,
						1.0f, 0.0f, 0.0f, 1.0f,
					};

					//Create VBO
					glGenBuffers(1, &gVBO);
					glBindBuffer(GL_ARRAY_BUFFER, gVBO);
					glBufferData(GL_ARRAY_BUFFER, 8 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

					//Create IBO
					glGenBuffers(1, &gCBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gCBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*4 * sizeof(GLfloat), vertexColorData, GL_STATIC_DRAW);
					//Create IBO
					glGenBuffers(1, &gIBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, 16 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
				}
			}
		}
	}

	return success;
}

void handleKeys(unsigned char key, int x, int y)
{
	//Toggle quad
	if (key == 'q')
	{
		gRenderQuad = !gRenderQuad;
	}
	if (key == 'w')
	{
		zoom += 0.2f;

	}
	if (key == 's')
	{
		zoom -= 0.2f;

	}
}

void update()
{
	//No per frame update needed
}

void render()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Render quad
	if (gRenderQuad)
	{
		//Bind program
		glUseProgram(gProgramID);

		//Enable vertex position
		glEnableVertexAttribArray(gVertexPos2DLocation);

		//Set vertex data
		glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		glVertexAttribPointer(gVertexPos2DLocation, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);

		glEnableVertexAttribArray(gVertexColorLocation);
		//Set vertex data
		glBindBuffer(GL_ARRAY_BUFFER, gCBO);
		glVertexAttribPointer(gVertexColorLocation, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);

		//Set index data and render
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
		GLint uniMvp = glGetUniformLocation(gProgramID, "mvp");
		GLint unisumCo = glGetUniformLocation(gProgramID, "sumCo");
		
		//for (float y = -2.0; y < 3.0f; y = y + 1.1f) {
		//	for (float x = -2.0; x < 3.0f; x = x + 1.1f) {
				glm::mat4 model(1.0);
				model = glm::scale(model, glm::vec3(0.2, 0.2f, 0.2));
				//model = glm::translate(model, glm::vec3(x, y, 0.0f));

				//if (rot1 > 100)  rot1 = 0;
				model = glm::rotate(
					model,
					glm::radians(rot1),
					glm::vec3(1.0f, 1.0f, 0.0f)
				);
				
				//model = glm::translate(model, glm::vec3(quat[0], quat[0], quat[0]));


				glm::mat4 view = glm::lookAt(
					glm::vec3(0.0f, 0.0f, 80.0f + zoom), // the position of your camera, in world space
					glm::vec3(0.0f, 0.0f, 0.0f), // where you want to look at, in world space
					glm::vec3(0.0f, 1.0f, 0.0f)); //upp

				glm::mat4 proj = glm::perspective(glm::radians(55.0f), 640.0f / 480.0f, 0.1f, 200.0f);

				glm::mat4 mvp = proj * view * model;
				glUniformMatrix4fv(uniMvp, 1, GL_FALSE, glm::value_ptr(mvp));
				glUniform3f(unisumCo,  color[0], color[1], color[2]);
				glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, NULL);

				//}
			//}
		//}
		//Disable vertex position
		glDisableVertexAttribArray(gVertexPos2DLocation);
		glDisableVertexAttribArray(gVertexColorLocation);

		//Unbind program
		glUseProgram(NULL);
	}
	
}

void close()
{
	//Deallocate program
	glDeleteProgram(gProgramID);
	TwTerminate();
	//Destroy window	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	
	//Quit SDL subsystems
	SDL_Quit();
}

void printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

#undef main

int main(int argc, char* args[])
{
	
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{  
		// Create a tweak bar
		CreateTweakBar();
		//Main loop flag
		bool quit = false;

		
		//Enable text input
		SDL_StartTextInput();
		

		//While application is running
		while (!quit)
		{
			rot1 += (float)SDL_GetTicks() / 25.0f * (3.14 / 180.0f);;
			render();
			TwDraw();

			//Update screen
			SDL_GL_SwapWindow(gWindow);
			//Event handler
			SDL_Event e;
			int handled;

			//Handle events on queue
			while (SDL_PollEvent(&e))
			{
				// Send event to AntTweakBar
				handled = TwEventSDL(&e, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);

				// If event has not been handled by AntTweakBar, process it
				//User requests quit
				if (!handled)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle keypress with current mouse position
					else if (e.type == SDL_TEXTINPUT)
					{
						int x = 0, y = 0;
						SDL_GetMouseState(&x, &y);
						handleKeys(e.text.text[0], x, y);
					}
				}
			}

			
		}

		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();
	
	return 0;
}