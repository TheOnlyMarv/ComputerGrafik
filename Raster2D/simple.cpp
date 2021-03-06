// 2D
// Simple framework for 2D-Drawing

#define FREEGLUT_STATIC
#define _USE_MATH_DEFINES

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <vector>

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

void drawCircle(int Radius, int xC, int yC)
{
	int P;
	int x, y;
	void drawCirclePoint(int x, int y, int xC, int yC);
	P = 1 - Radius;
	x = 0;
	y = Radius;
	drawCirclePoint(x, y, xC, yC);
	while (x <= y)
	{
		x++;
		if (P < 0)
		{
			P += 2 * x + 1;
		}
		else
		{
			P += 2 * (x - y) + 1;
			y--;
		}
		drawCirclePoint(x, y, xC, yC);
	}

}

void drawCirclePoint(int x, int y, int xC, int yC)
{
	setPixel(xC + x, yC + y, 0.0f, 1.0f, 0.0f);
	setPixel(xC + x, yC - y, 0.0f, 1.0f, 0.0f);
	setPixel(xC - x, yC + y, 0.0f, 1.0f, 0.0f);
	setPixel(xC - x, yC - y, 0.0f, 1.0f, 0.0f);
	setPixel(xC + y, yC + x, 0.0f, 1.0f, 0.0f);
	setPixel(xC - y, yC + x, 0.0f, 1.0f, 0.0f);
	setPixel(xC + y, yC - x, 0.0f, 1.0f, 0.0f);
	setPixel(xC - y, yC - x, 0.0f, 1.0f, 0.0f);
}

struct Point {
	int x, y;
	Point() {};
	Point(int x, int y) : x(x), y(y) {
	}

	Point Point::operator+(const Point pt) const
	{
		Point temp;
		temp.x = x + pt.x;
		temp.y = y + pt.y;
		return temp;
	}

	Point Point::operator*(double m) const
	{
		Point temp;
		temp.x = x*m;
		temp.y = y*m;
		return temp;
	}
};

void Casteljau(Point* points, int n) {
	// Strange instantiate, because for normal instantiation, you need a constant n
	Point** temp = new Point*[n];
	for (int i = 0; i < n; i++)
		temp[i] = new Point[n];

	for (double t = 0; t <= 1.0; t += 0.0001)
	{
		for (int i = 0; i < n; i++)
		{
			temp[i][0] = points[i];
		}
		for (int j = 1; j < n; j++)
		{
			for (int i = j; i < n; i++)
			{
				temp[i][j].x = (1 - t) * temp[i - 1][j - 1].x + t*temp[i][j - 1].x;
				temp[i][j].y = (1 - t) * temp[i - 1][j - 1].y + t*temp[i][j - 1].y;
			}
		}
		setPixel(temp[n - 1][n - 1].x, temp[n - 1][n - 1].y, 0.0f, 1.0f, 0.0f);
	}
}

void fillRectangle(Point p0, Point p1) {
	for (int y = p0.y; y <= p1.y; y++)
	{
		for (int x = p0.x; x <= p1.x; x++)
		{
			/*std::cout << x << " " << y;*/
			setPixel(x, y, 0.0f, 1.0f, 0.0f);
		}
	}
}

void fillTriangle(Point p1, Point p2, Point p3) {
	int minX = min(p1.x, min(p2.x, p3.x));
	int minY = min(p1.y, min(p2.y, p3.y));
	int maxX = max(p1.x, max(p2.x, p3.x));
	int maxY = max(p1.y, max(p2.y, p3.y));

	int f0 = (p1.y - p2.y) * (minX - p1.x) + (p2.x - p1.x) * (minY - p1.y);
	int f1 = (p2.y - p3.y) * (minX - p2.x) + (p3.x - p2.x) * (minY - p2.y);
	int f2 = (p3.y - p1.y) * (minX - p3.x) + (p1.x - p3.x) * (minY - p3.y);

	for (int y = minY; y <= maxY; y++)
	{
		int ff0 = f0, ff1 = f1, ff2 = f2;
		for (int x = minX; x < maxX; x++)
		{
			if (ff0 >= 0 && ff1 >= 0 && ff2 >= 0) {
				float alpha = ((p2.y - p3.y)*(x - p3.x) + (p3.x - p2.x)*(y - p3.y)) / (float)((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
				float beta = ((p3.y - p1.y)*(x - p3.x) + (p1.x - p3.x)*(y - p3.y)) / (float)((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
				float gamma = 1.0f - alpha - beta;
				setPixel(x, y, gamma, alpha, beta);
			}
			ff0 = ff0 + (p1.y - p2.y);
			ff1 = ff1 + (p2.y - p3.y);
			ff2 = ff2 + (p3.y - p1.y);
		}
		f0 = f0 + (p2.x - p1.x);
		f1 = f1 + (p3.x - p2.x);
		f2 = f2 + (p1.x - p3.x);
	}
}

struct EdgePassiv {
	float minY, minX, maxY, maxX;
	EdgePassiv(float minY, float minX, float maxY, float maxX): minY(minY), minX(minX), maxY(maxY), maxX(maxX) {

	}
};

struct EdgeAktiv {
	float xs, dx, maxY;
	EdgeAktiv(float xs, float dx, float maxY): xs(xs), dx(dx), maxY(maxY) {

	}
};

int compPassiv(const void *one, const void *two)
{
	EdgePassiv a = *((EdgePassiv*)one);
	EdgePassiv b = *((EdgePassiv*)two);
	if (a.minY<b.minY)
		return -1;
	if (a.minY == b.minY)
		return 0;
	return 1;
}

int compActiv(const void *one, const void *two)
{
	EdgeAktiv a = *((EdgeAktiv*)one);
	EdgeAktiv b = *((EdgeAktiv*)two);
	if (a.xs<b.xs)
		return -1;
	if (a.xs == b.xs)
		return 0;
	return 1;
}

std::vector<EdgePassiv> createEdges(Point* points, int n) {
	std::vector<EdgePassiv> edgesPassiv;
	for (int i = 0; i < n; i++)
	{
		int j = (i + 1) % n;
		float minY = min(points[i].y, points[j].y), minX = min(points[i].x, points[j].x), maxY = max(points[i].y, points[j].y), maxX = max(points[i].x, points[j].x);
		edgesPassiv.push_back(EdgePassiv(minY, minX, maxY, maxX));
	}
	return edgesPassiv;
}

void fillPolygon(Point* points, int n) {
	int minY = INT32_MAX, minX = INT32_MAX, maxY = 0, maxX = 0;
	for (int i = 0; i < n; i++)
	{
		minY = min(minY, points[i].y);
		maxY = max(maxY, points[i].y);
		minX = min(minX, points[i].x);
		maxX = max(maxX, points[i].x);
	}
	
	std::vector<EdgePassiv> edgesPassiv = createEdges(points, n);
	std::vector<EdgeAktiv> edgesActiv;

	std::qsort(&edgesPassiv[0], edgesPassiv.size(), sizeof(EdgePassiv), compPassiv);

	for (int y = minY; y <= maxY; y++)
	{
		for (int i = 0; i < edgesPassiv.size(); i++)
		{
			if (y == edgesPassiv[i].minY) {
				EdgePassiv ep = edgesPassiv[i]; edgesPassiv.erase(edgesPassiv.begin() + i);
				i--;
				edgesActiv.push_back(EdgeAktiv(ep.minX, (ep.maxX - ep.minX) / (ep.maxY - ep.minY), ep.maxY));
			}
		}
		for (int i = 0; i < edgesActiv.size(); i++)
		{
			if (y == edgesActiv[i].maxY) {
				edgesActiv.erase(edgesActiv.begin() + i);
				i--;
			}
		}
		qsort(&edgesActiv[0], edgesActiv.size(), sizeof(EdgeAktiv), compActiv);

		for (int i = 0; i < edgesActiv.size(); i+=2)
		{
			for (int x = edgesActiv[i].xs; x <= edgesActiv[i + 1].xs; x++) {
				setPixel(x, y, 0.5f, 1.0f, 0.0f);
			}
		}

		for (int i = 0; i < edgesActiv.size(); i++) {
			edgesActiv[i].xs += edgesActiv[i].dx;
		}
	}
}

void testCasteljau() {
	Point points[] = { Point(100,100), Point(100,150), Point(200, 150), Point(200, 100), Point(230, 50) };
	Casteljau(points, sizeof(points) / sizeof(Point));
	writeToPPM("testCasteljau.ppm");
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

void testCircle()
{
	drawCircle(100, 128, 128);
	writeToPPM("testCircle.ppm");
}

void testFilledRectangle() {
	fillRectangle(Point(200, 200), Point(220, 240));
	writeToPPM("testFilledRectangle.ppm");
}

void testFillTriangle() {
	fillTriangle(Point(100, 100), Point(200, 100), Point(150, 200));
	glFlush();
	writeToPPM("testFillTriangle.ppm");
}

void testFillPolygon() {
	Point points[] = { Point(50, 50), Point(100, 100), Point(60, 120) };
	fillPolygon(points, sizeof(points) / sizeof(Point));
	glFlush();
	writeToPPM("testFillPolygon.ppm");
}


////////////////////////////////////////////////////////////
// WU Algorithm
GLfloat wuR = 100, wuG = 40, wuB = 90;
void plot(int x, int y, GLfloat c) {
	setPixel(x, y, c, c, c);
}

float ipart(float x) {
	return (int)x;
}

float roundi(float x) {
	return ipart(x + 0.5);
}

float fpart(float x) {
	if (x < 0) {
		return 1 - (x - floor(x));
	}
	else
	{
		return x - floor(x);
	}
}

float rfpart(float x) {
	return 1 - fpart(x);
}

void drawWuLine(int x0, int y0, int x1, int y1) {
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		int temp = x0;
		x0 = y0;
		y0 = temp;
		temp = x1;
		x1 = y1;
		y1 = temp;
	}
	if (x0 > x1) {
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = y0;
	}

	float dx = x1 - x0;
	float dy = y1 - y0;
	float gradient = dy / (float)dx;

	float xend = roundi(x0);
	float yend = y0 + gradient * (xend - x0);
	float xgap = rfpart(x0 + 0.5);
	float xpxl1 = xend;
	float ypxl1 = ipart(yend);
	if (steep) {
		plot(ypxl1, xpxl1, rfpart(yend) * xgap);
		plot(ypxl1 + 1, xpxl1, fpart(yend) * xgap);
	}
	else
	{
		plot(xpxl1, ypxl1, rfpart(yend) * xgap);
		plot(xpxl1, ypxl1 + 1, fpart(yend) * xgap);
	}

	float intery = yend + gradient;

	xend = roundi(x1);
	yend = y1 + gradient * (xend - x1);
	xgap = fpart(yend);
	float xpxl2 = xend;
	float ypxl2 = ipart(yend);
	if (steep)
	{
		plot(ypxl2, xpxl2, rfpart(yend) * xgap);
		plot(ypxl2 + 1, xpxl2, fpart(yend) * xgap);
	}
	else
	{
		plot(xpxl2, ypxl2, rfpart(yend) * xgap);
		plot(xpxl2, ypxl2 + 1, fpart(yend) * xgap);
	}

	for (int x = xpxl1; x < xpxl2; x++)
	{
		if (steep) {
			plot(ipart(intery), x, rfpart(intery));
			plot(ipart(intery) + 1, x, fpart(intery));
		}
		else
		{
			plot(x, ipart(intery), rfpart(intery));
			plot(x, ipart(intery) + 1, fpart(intery));
		}
		intery = intery + gradient;
	}

}



void testWuLine() {
	drawWuLine(20, 20, 60, 30);
	drawBresenhamLineOriginal(60, 20, 100, 30);
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

	//test1();
	//test2(); //fancy star
	//testCircle();
	testCasteljau();
	//testFilledRectangle();
	//testFillTriangle();
	//testFillPolygon();
	//testWuLine();

	/////////////////////////////////
	glutMainLoop();

	return 0;
}