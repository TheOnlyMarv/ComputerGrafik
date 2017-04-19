// Simple.cpp
// The Simplest OpenGL program with GLUT

#define FREEGLUT_STATIC

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>

GLfloat rectangleX = 0.0f;
GLfloat rectangleY = 0.0f;
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
	glLoadIdentity();

	glTranslatef(rectangleX, rectangleY, 0);
	glRotatef(rotateAngle, 0, 0, 1);
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(-0.25f, 0.25f, 0.25f, -0.25f);

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
	if (rectangleX > 1 - 0.25 || rectangleX < -1 + 0.25)
		xstep = -xstep;
	if (rectangleY > 1 - 0.25 || rectangleY < -1 + 0.25)
		ystep = -ystep;
	
	rectangleX += xstep;
	rectangleY += ystep;
	rotateAngle += rotateStep;
	rotateAngle = (rotateAngle > 360 ? 0 : (rotateAngle < 0 ? 360 : rotateAngle));

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

