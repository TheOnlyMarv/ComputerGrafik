#define FREEGLUT_STATIC

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>
#include "robo.h"


void RenderSceneRoboter() {
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);
	//drawViewPortBorder();
	glLoadIdentity();
}