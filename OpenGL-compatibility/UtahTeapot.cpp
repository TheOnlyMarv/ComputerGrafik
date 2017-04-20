#define FREEGLUT_STATIC

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>
#include <iostream>
#include "UtahTeapot.h"


float translationStep = 0.04f;
float rotationStep = 2.0f;
float transX = 0.0f;
float transY = 0.0f;
float angleY = 0.0f;
float angleX = 0.0f;

///////////////////////////////////////////////////////////
// Teapot render callback
void RenderSceneTeapot(void) {
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);
	//drawViewPortBorder();
	glLoadIdentity();

	glTranslatef(transX, transY, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleX, 1, 0, 0);
	glutWireTeapot(0.5f);

	// Flush drawing commands
	glutSwapBuffers();
}

enum TranslationDirection
{
	Up,
	Down,
	Left,
	Right
};

enum RotationDirection
{
	RLeft,
	RRight,
	RUp,
	RDown
};

void TranslateTeapot(TranslationDirection ts) {
	switch (ts)
	{
	case Up:
		transY += translationStep;
		break;
	case Down:
		transY -= translationStep;
		break;
	case Left:
		transX -= translationStep;
		break;
	case Right:
		transX += translationStep;
		break;
	default:
		break;
	}
}

void RotateTeapot(RotationDirection rd) {
	switch (rd)
	{
	case RLeft:
		angleY += rotationStep;
		break;
	case RRight:
		angleY -= rotationStep;
		break;
	case RUp:
		angleX += rotationStep;
		break;
	case RDown:
		angleX -= rotationStep;
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////
// Teapot keyboard callback
void TeapotKeyboard(unsigned char c, int i0, int i1) {
	switch (c)
	{
	case 'a':
		TranslateTeapot(Left);
		break;
	case 'd':
		TranslateTeapot(Right);
		break;
	case 'w':
		TranslateTeapot(Up);
		break;
	case 's':
		TranslateTeapot(Down);
		break;
	case 'q':
		RotateTeapot(RLeft);
		break;
	case 'e':
		RotateTeapot(RRight);
		break;
	case 'r':
		RotateTeapot(RUp);
		break;
	case 'f':
		RotateTeapot(RDown);
		break;
	default:
		break;
	}
	//std::cout << c << std::endl;
}