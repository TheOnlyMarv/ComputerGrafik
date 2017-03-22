// 2D
// Simple framework for 2D-Drawing

#define FREEGLUT_STATIC
#define _USE_MATH_DEFINES

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <math.h>

const int RESOLUTION = 256;

struct RGBPixel {
	GLfloat R, G, B;
	RGBPixel(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0) : R(r), G(g), B(b) {};
};

RGBPixel framebuffer[RESOLUTION][RESOLUTION];

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
	framebuffer[x < 0 ? 0 : (x >= RESOLUTION ? RESOLUTION - 1 : x)][y < 0 ? 0 : (y >= RESOLUTION ? RESOLUTION - 1 : y)] =
		RGBPixel(r, g, b);
	glutPostRedisplay();
}

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat d = 2.0f / GLfloat(RESOLUTION);
	for (int y = 0; y < RESOLUTION; y++)
		for (int x = 0; x < RESOLUTION; x++) {
			const RGBPixel& p = framebuffer[x][y];
			if (p.R == 0 && p.G == 0 && p.B == 0)
				continue;
			glColor3f(p.R, p.G, p.B);
			GLfloat vpx = GLfloat(x) * 2.0f / GLfloat(RESOLUTION) - 1.0f;
			GLfloat vpy = GLfloat(y) * 2.0f / GLfloat(RESOLUTION) - 1.0f;
			glRectf(vpx, vpy, vpx + d, vpy + d);
		}

	// Flush drawing commands
	glFlush();
}

///////////////////////////////////////////////////////////
// Setup the rendering state
void SetupRC(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void KeyPress(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q': exit(1);
	}
}

void drawFancyLine()
{
	for (int n = 0; n < RESOLUTION; ++n)
	{
		setPixel(n, n, GLfloat(n) / RESOLUTION, 0, 1.0f - GLfloat(n) / RESOLUTION);
	}
}

void drawSomePixels()
{
	setPixel(10, 10, 1.0f, 0.0f, 0.0f);
	setPixel(20, 10, 1.0f, 1.0f, 0.0f);
	setPixel(10, 20, 0.0f, 1.0f, 0.0f);
	setPixel(20, 20, 0.0f, 0.0f, 1.0f);
}

void writeToPPM(char* fileName)
{
	std::ofstream fileStream;
	fileStream.open(fileName);

	fileStream << "P3\n";
	fileStream << RESOLUTION << " " << RESOLUTION << "\n";
	fileStream << 255 << "\n";

	int numberOfPixels = RESOLUTION * RESOLUTION;

	for (int i = 0; i < numberOfPixels; ++i)
	{
		if ((i % 4) == 0 && i > 0)
		{
			fileStream << "\n";
		}

		int x = i % RESOLUTION;
		int y = RESOLUTION - (i / RESOLUTION);

		RGBPixel currentPixel = framebuffer[x][y];
		int r = (int)(currentPixel.R * 255);
		int g = (int)(currentPixel.G * 255);
		int b = (int)(currentPixel.B * 255);

		fileStream << r << " ";
		fileStream << g << " ";
		fileStream << b << " ";
	}
	fileStream.close();
}

void drawBresenhamLineOriginal(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int const1 = dx + dx;
	int const2 = dy + dy;
	int error = const2 - dx;

	while (x1 <= x2)
	{
		setPixel(x1, y1, 1.0f, 1.0f, 1.0f);
		++x1;

		if (error <= 0)
		{
			error += const2;
		}
		else
		{
			++y1;
			error += const2 - const1;
		}
	}
}

void drawBresenhamLine(int x1, int y1, int x2, int y2, GLfloat r, GLfloat g, GLfloat b)
{
	int x = x1;
	int y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int inc_x = dx > 0 ? 1 : -1;
	int inc_y = dy > 0 ? 1 : -1;

	if (abs(dy) < abs(dx))
	{
		int error = -abs(dx);
		int delta = 2 * abs(dy);
		int step = 2 * error;

		while (x != x2)
		{
			setPixel(x, y, r, g, b);
			x += inc_x;
			error += delta;

			if (error > 0)
			{
				y += inc_y;
				error += step;
			}
		}
	}
	else
	{
		int error = -abs(dy);
		int delta = 2 * abs(dx);
		int step = 2 * error;

		while (y != y2) {
			setPixel(x, y, r, g, b);
			y += inc_y;
			error += delta;

			if (error > 0)
			{
				x += inc_x;
				error += step;
			}
		}
	}
	setPixel(x2, y2, r, g, b);
}

void drawMidpointLine(int x1, int y1, int x2, int y2, GLfloat r, GLfloat g, GLfloat b)
{
	int x = x1;
	int y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int f = dy - dx / 2;

	for (int i = 0; i < dx; ++i)
	{
		setPixel(x, y, r, g, b);
		++x;

		if (f > 0)
		{
			y += 1;
			f -= dx;
		}
		f += dy;
	}
}

RGBPixel getColor(float colorValue)
{
	colorValue = 0.1 + colorValue * 0.9;
	return RGBPixel(colorValue, colorValue, colorValue);
}

void drawSun(int numberOfLines)
{
	int mid_x = round(RESOLUTION / 2.0f);
	int mid_y = round(RESOLUTION / 2.0f);
	float radius = 100.0f;

	for (int i = 0; i < numberOfLines; ++i)
	{
		double angle = i * M_PI / (numberOfLines / 2.0f);
		int x2 = mid_x + round(radius * cos(angle));
		int y2 = mid_y + round(radius * sin(angle));

		float colorValue = (float)i / numberOfLines;
		RGBPixel currentLineColor = getColor(colorValue);

		drawBresenhamLine(mid_x, mid_y, x2, y2, currentLineColor.R, currentLineColor.G, currentLineColor.B);
	}
}

void test1()
{
	drawFancyLine();
	drawSomePixels();
	drawBresenhamLineOriginal(80, 120, 150, 140);
	drawBresenhamLine(200, 20, 180, 180, 1.0f, 1.0f, 1.0f);
	drawMidpointLine(90, 120, 160, 140, 1.0f, 1.0f, 1.0f);
	writeToPPM("testA.ppm");
}

void test2()
{
	drawSun(16);
	writeToPPM("testB.ppm");
}

///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("2D-Test");
	glutReshapeWindow(512, 512);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(KeyPress);

	SetupRC();
	////////// put your framebuffer drawing code here /////////////
	test1();
	test2();
	/////////////////////////////////
	glutMainLoop();

	return 0;
} 