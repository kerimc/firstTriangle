/* use the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
// use glew instead od gl.h to get all GL prototype
#include "GL\glew.h"
// use the freegult library for base windowing setup
#include "GL\freeglut.h"

int init_resources()
{
	return 1;
}
void onDisplay()
{
	//fill in later
}
void free_resources()
{
	// fill later
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






