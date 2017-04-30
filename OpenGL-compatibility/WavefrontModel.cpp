//#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
//#include "tiny_obj_loader.h"
#define FREEGLUT_STATIC
//#define GLEW_STATIC

#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)
#include <vector>

#include <iostream>
#include <math.h>
#include "WavefrontModel.h"

std::vector<Vertex> vertices;

float translationStepModel = 0.04f;
float rotationStepModel = 2.0f;
float zoomStepModel = 0.04f;

float transXModel = 0.0f;
float transYModel = 0.0f;
float angleYModel = 0.0f;
float angleXModel = 0.0f;
float zoomZModel = std::numeric_limits<float>::max();
#define min(a,b)            (((a) < (b)) ? (a) : (b))

bool loadOBJ(const char * path, std::vector<Vertex>& out_vertices)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< Vertex > temp_vertices;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	std::cout << "Loading " << path << std::endl;
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			Vertex vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			zoomZModel = min(zoomZModel, vertex.z);
			temp_vertices.push_back(vertex);

		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
			//std::cout << matches << std::endl;
			if (matches == 3)
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
			}
			else {
				matches = fscanf(file, "/%d %d/%d %d/%d", &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
				//std::cout << matches << std::endl;
				if (matches == 5) {
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
				}
				else {
					matches = fscanf(file, "/%d %d/%d/%d %d/%d/%d\n", &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					//std::cout << matches << std::endl;
					if (matches != 7) {
						printf("File can't be read by our simple parser : ( Try exporting with other options\n");
						return false;
					}
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);

				}
			}


		}
	}
	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		Vertex vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	vertices = out_vertices;
	std::cout << "Loading complete" << std::endl;

}

GLfloat fAspectModel;

void RenderSceneModel(void) {
	glClear(GL_COLOR_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, fAspectModel, 0.0, 400.0);
	glTranslatef(0.0f, 0.0f, zoomZModel);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(transXModel, transYModel, 0);
	glRotatef(angleYModel, 0, 1, 0);
	glRotatef(angleXModel, 1, 0, 0);


	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		glVertex3f(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);
		glVertex3f(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
		glEnd();
	}
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glutSwapBuffers();
}


void ModelViewportParallel(GLsizei w, GLsizei h) {
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


///////////////////////////////////////////////////////////
// Roboter Viewport
void ModelChangeView(GLsizei w, GLsizei h) {
	//ModelViewportParallel(w, h);

	// Set Viewport towindowdimensions
	glViewport(0, 0, w, h);
	fAspectModel = (GLfloat)w / (GLfloat)h;
	// Resetcoordinatesystem
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Producetheperspectiveprojection
	gluPerspective(60.0f, fAspectModel, 0.0, 400.0);
	glTranslatef(0.0f, 0.0f, zoomZModel);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

void TranslateModel(TranslationDirection ts) {
	switch (ts)
	{
	case Up:
		transYModel += translationStepModel;
		break;
	case Down:
		transYModel -= translationStepModel;
		break;
	case Left:
		transXModel -= translationStepModel;
		break;
	case Right:
		transXModel += translationStepModel;
		break;
	default:
		break;
	}
}

void RotateModel(RotationDirection rd) {
	switch (rd)
	{
	case RLeft:
		angleYModel += rotationStepModel;
		break;
	case RRight:
		angleYModel -= rotationStepModel;
		break;
	case RUp:
		angleXModel += rotationStepModel;
		break;
	case RDown:
		angleXModel -= rotationStepModel;
		break;
	default:
		break;
	}
}

enum CameraAction {
	ZoomIn,
	ZoomOut
};

void CameraTranslationModel(CameraAction ca) {
	switch (ca)
	{
	case ZoomIn:
		zoomZModel += zoomStepModel;
		break;
	case ZoomOut:
		zoomZModel -= zoomStepModel;
		break;
	default:
		break;
	}
}


///////////////////////////////////////////////////////////
// Model keyboard callback
void ModelKeyboard(unsigned char c, int i0, int i1) {
	switch (c)
	{
	case 'a':
		TranslateModel(Left);
		break;
	case 'd':
		TranslateModel(Right);
		break;
	case 'w':
		TranslateModel(Up);
		break;
	case 's':
		TranslateModel(Down);
		break;
	case 'q':
		RotateModel(RLeft);
		break;
	case 'e':
		RotateModel(RRight);
		break;
	case 'r':
		RotateModel(RUp);
		break;
	case 'f':
		RotateModel(RDown);
		break;
	case '5':
		CameraTranslationModel(ZoomIn);
		break;
	case '2':
		CameraTranslationModel(ZoomOut);
		break;

	default:
		break;
	}
}

//std::vector<Vertex>;

//std::string inputfile = "cornell_box.obj";
//tinyobj::attrib_t attrib;
//std::vector<tinyobj::shape_t> shapes;
//std::vector<tinyobj::material_t> materials;
//
//std::string err;
//bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
//
//
//bool TestLoadObj(const char* filename, const char* basepath = NULL,
//	bool triangulate = true) {
//	std::cout << "Loading " << filename << std::endl;
//
//	tinyobj::attrib_t attrib;
//	std::vector<tinyobj::shape_t> shapes;
//	std::vector<tinyobj::material_t> materials;
//
//	std::string err;
//	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename,
//		basepath, triangulate);
//	printf("Parsing time:  [msecs]\n");
//
//	if (!err.empty()) {
//		std::cerr << err << std::endl;
//	}
//
//	if (!ret) {
//		printf("Failed to load/parse .obj.\n");
//		return false;
//	}
//
//	char* test;
//	std::cin >> test;
//	return true;
//}