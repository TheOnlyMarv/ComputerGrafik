#define FREEGLUT_STATIC

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>
#include <iostream>
#include "Robot.h"

struct RobotPart
{
	float x, y;
	float width, height;

	RobotPart(float width, float height) : width(width), height(height)
	{
		x = -(width / 2);
		y = -(height / 2);
	}
};

RobotPart base = RobotPart(0.5f, 0.25f);
float HEIGHT_BASE = 0.25f;
float HEIGHT_UPPER_ARM = 1.0f;
float X_SIZE_LOWER_ARM = 0.125f;

///////////////////////////////////////////////////////////
// Roboter render callback
void RoboterRenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	glLoadIdentity();
	// Basis zeichnen
	//glTranslatef(tr_base_x, tr_base_y, …);
	//Befehle zum Zeichnen der Basis
	glScalef(1, HEIGHT_BASE, 1);
	glColor3f(0.0f, 0.5f, 1.0f);
	glutSolidCube(1.0);


	// Oberarm zeichnen
	glTranslatef(0, HEIGHT_BASE, 0);
	//glRotatef(alpha, 0, 1, 0);
	//Befehle zum Zeichnen des Oberarm
	glScalef(0.25, HEIGHT_UPPER_ARM, 1);
	glColor3f(0.0f, 0.5f, 1.0f);
	glutSolidCube(1.0);

	// Unterarm zeichnen
	glTranslatef(0, HEIGHT_UPPER_ARM, 0);
	//glRotatef(beta, 1, 0, 0);
	//Befehle zum Zeichnen des Unterarms
	glScalef(X_SIZE_LOWER_ARM, 0.25, 1);
	glColor3f(0.0f, 0.5f, 1.0f);
	glutSolidCube(1.0);


	glTranslatef(X_SIZE_LOWER_ARM, 0, 0);
	// Hand zeichnen
	//Befehle zum Zeichnen der Hand

	////Base
	//glScalef(1, 0.25, 0.25);
	//glColor3f(0.0f, 0.5f, 1.0f);
	//glutSolidCube(1.0);
	//
	//glLoadIdentity();
	//// Right arm
	//glScalef(0.25, 1.0f, 1.0f);
	//glutSolidCube(1.0);
	glutSwapBuffers();
}

///////////////////////////////////////////////////////////
// Roboter Viewport
void RoboterChangeView(GLsizei w, GLsizei h) {
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