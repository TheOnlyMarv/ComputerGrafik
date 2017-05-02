//#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
//#include "tiny_obj_loader.h"
#define FREEGLUT_STATIC
//#define GLEW_STATIC

#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)
#include <vector>

#include <iostream>
#include <math.h>
#include "ObjectLoader.h"

class Vertex
{
public:
	float x, y, z, w = 1.0f;
};


GLfloat* loadOBJ(const char * path, long & n)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< Vertex > temp_vertices;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return nullptr;
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
						return nullptr;
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

	long vn = vertexIndices.size() * 4;
	GLfloat* out_vertices = new GLfloat[vn]();
	// For each vertex of each triangle
	long j = 0;
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		Vertex vertex = temp_vertices[vertexIndex - 1];
		out_vertices[j++] = vertex.x;
		out_vertices[j++] = vertex.y;
		out_vertices[j++] = vertex.z;
		out_vertices[j++] = vertex.w;
	}
	std::cout << "Loading complete" << std::endl;
	n = j;
	return out_vertices;
}