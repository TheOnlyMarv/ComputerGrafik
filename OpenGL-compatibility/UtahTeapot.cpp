#define FREEGLUT_STATIC

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>
#include <iostream>
#include "UtahTeapot.h"


float translationStep = 0.04f;
float rotationStep = 2.0f;
float zoomStep = 0.04f;

float transX = 0.0f;
float transY = 0.0f;
float angleY = 0.0f;
float angleX = 0.0f;
float zoomZ = -1.0f;

GLfloat fAspect;

///////////////////////////////////////////////////////////
// Teapot render callback
void RenderSceneTeapot(void) {
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);
	//drawViewPortBorder();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, fAspect, 0.0, 400.0);
	glTranslatef(0.0f, 0.0f, zoomZ);
	//glTranslatef(0.0f, 0.0f, zoomZ);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(transX, transY, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleX, 1, 0, 0);
	glutWireTeapot(0.5f);

	// Flush drawing commands
	glutSwapBuffers();
}

void ViewportParallel(GLsizei w, GLsizei h) {
	// Set Viewport towindowdimensions
	glViewport(0, 0, w, h);
	// Resetcoordinatesystem
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// CalcaspectRatio(Seitenverhältnis)
	GLfloat ar = (GLfloat)w / (GLfloat)h;
	if (w <= h)
		glOrtho(-1.0, 1.0, -1.0 / ar, 1.0 / ar, 1.0, -1.0);
	else
		glOrtho(-1.0*ar, 1.0*ar, -1.0, 1.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ViewportPerspectiv(GLsizei w, GLsizei h){
	// Set Viewport towindowdimensions
	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w / (GLfloat)h;
	// Resetcoordinatesystem
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Producetheperspectiveprojection
	gluPerspective(60.0f, fAspect, 0.0, 400.0);
	glTranslatef(0.0f, 0.0f, zoomZ);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TeapotChangeView(GLsizei w, GLsizei h) {
	ViewportPerspectiv(w, h);
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

enum CameraAction {
	ZoomIn,
	ZoomOut
};

void CameraTranslation(CameraAction ca) {
	switch (ca)
	{
	case ZoomIn:
		zoomZ += zoomStep;
		break;
	case ZoomOut:
		zoomZ -= zoomStep;
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
	case '5':
		CameraTranslation(ZoomIn);
		break;
	case '2':
		CameraTranslation(ZoomOut);
		break;

	default:
		break;
	}
	std::cout << c << std::endl;
}