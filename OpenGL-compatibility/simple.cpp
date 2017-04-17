// Simple.cpp
// The Simplest OpenGL program with GLUT

#define FREEGLUT_STATIC

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>

GLfloat x = 0.0f;
GLfloat y = 0.0f;

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(x, y, x + 0.25f, y + 0.25f);

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
GLfloat xstep = 0.03f;
GLfloat ystep = 0.02f;

void BouncingRectangle(int value) {
	if (x > 1 - 0.25 || x < -1 + 0.25)
		xstep = -xstep;
	if (y > 1 - 0.25 || y < -1 + 0.25)
		ystep = -ystep;
	x += xstep;
	y += ystep;
	glutPostRedisplay();
	glutTimerFunc(100, BouncingRectangle, 1);
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

