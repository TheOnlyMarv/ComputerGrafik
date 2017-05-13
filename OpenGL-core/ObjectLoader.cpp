//#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
//#include "tiny_obj_loader.h"
#define FREEGLUT_STATIC
//#define GLEW_STATIC

#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)
#include "glm\common.hpp"
#include "glm\gtx\normal.hpp"

#include <iostream>
#include <math.h>
#include "ObjectLoader.h"

using namespace glm;

void loadOBJ(const char * path, std::vector<GLfloat> & out_vertices)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< vec4 > temp_vertices;
	std::vector< vec3 > temp_normals;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return;
	}

	std::cout << "Loading " << path << std::endl;
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			vec4 vertex = vec4(1.0f);
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
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
						return;
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
		if (normalIndices.size() == vertexIndices.size())
		{
			long j = 0;
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
				unsigned int vertexIndex = vertexIndices[i];
				vec4 vertex = temp_vertices[vertexIndex - 1];
				out_vertices.push_back(vertex.x);
				out_vertices.push_back(vertex.y);
				out_vertices.push_back(vertex.z);
				out_vertices.push_back(vertex.w);

				unsigned int normalIndex = normalIndices[i];
				vec3 normal = temp_normals[normalIndex - 1];
				out_vertices.push_back(normal.x);
				out_vertices.push_back(normal.y);
				out_vertices.push_back(normal.z);
			}
		}
		else
		{
			for (unsigned int i = 0; i < vertexIndices.size(); i += 3)
			{
				vec4 v1 = temp_vertices[vertexIndices[i] - 1];
				vec4 v2 = temp_vertices[vertexIndices[i + 1] - 1];
				vec4 v3 = temp_vertices[vertexIndices[i + 2] - 1];
				vec3 normal = glm::triangleNormal(
					vec3(v1),
					vec3(v2),
					vec3(v3)
				);

				out_vertices.push_back(v1.x);
				out_vertices.push_back(v1.y);
				out_vertices.push_back(v1.z);
				out_vertices.push_back(v1.w);

				out_vertices.push_back(normal.x);
				out_vertices.push_back(normal.y);
				out_vertices.push_back(normal.z);

				out_vertices.push_back(v2.x);
				out_vertices.push_back(v2.y);
				out_vertices.push_back(v2.z);
				out_vertices.push_back(v2.w);

				out_vertices.push_back(normal.x);
				out_vertices.push_back(normal.y);
				out_vertices.push_back(normal.z);

				out_vertices.push_back(v3.x);
				out_vertices.push_back(v3.y);
				out_vertices.push_back(v3.z);
				out_vertices.push_back(v3.w);

				out_vertices.push_back(normal.x);
				out_vertices.push_back(normal.y);
				out_vertices.push_back(normal.z);
			}
		}

	std::cout << "Loading complete" << std::endl;
}