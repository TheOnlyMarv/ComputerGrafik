// Simple.cpp
// The Simplest OpenGL program with GLUT

#define FREEGLUT_STATIC

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat rotateAngle = 0.0f;

///////////////////////////////////////////////////////////
// Draws the border of the Viewingport
void drawViewPortBorder() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glRectf(-1.0, 1.0, 1.0, -1.0);
}

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);
	//drawViewPortBorder();
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(rotateAngle, 0, 0, 1);
	glTranslatef(-x, -y, 0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(x, y, x + 0.25f, y + 0.25f);
	glPopMatrix();

	// Flush drawing commands
    glutSwapBuffers();
}

///////////////////////////////////////////////////////////
// Setup the rendering state
void SetupRC(void)
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

///////////////////////////////////////////////////////////
// For Changing Viewport (Window size)
void ChangeView(GLsizei width, GLsizei height) {
	if (width > height) {
		glViewport(0, 0, height, height);
	}
	else {
		glViewport(0, 0, width, width);
	}
}

///////////////////////////////////////////////////////////
// Bouncing Rectangle
GLfloat xstep = 0.02f;
GLfloat ystep = 0.03f;
GLfloat rotateStep = 3.0f;

void BouncingRectangle(int value) {
	if (x > 1 - 0.25 || x < -1 + 0.25)
		xstep = -xstep;
	if (y > 1 - 0.25 || y < -1 + 0.25)
		ystep = -ystep;
	if (rotateAngle > 360)
		rotateAngle = 0;
	x += xstep;
	y += ystep;
	rotateAngle += rotateStep;
	//glTranslatef(1, 1, 1);
	glutPostRedisplay();
	glutTimerFunc(33, BouncingRectangle, 1);
}

///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 	glutCreateWindow("Simple");
	glutReshapeFunc(ChangeView);
	glutDisplayFunc(RenderScene);
	SetupRC();

	// For Bouncing Rectangle
	glutTimerFunc(33, BouncingRectangle, 1);

	glutMainLoop();
    
    return 0;
}

