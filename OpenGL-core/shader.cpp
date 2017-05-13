// Shader.cpp
// The Simplest OpenGL shader program with GLUT

#define FREEGLUT_STATIC

#include <windows.h>		// Must have for Windows platform builds
#include <stdio.h>
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)
#include "glm\glm.hpp";
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <iostream>

#include "ObjectLoader.h"

#include <math.h>
#include <string>
#include <vector>

#include <fstream>
#include <sstream>

using namespace std;

GLfloat rotx = 0;
GLfloat roty = 0;

glm::vec3 rot(0.0f);
glm::vec3 trans(0.0f);
glm::vec3 scale(0.01f);

//const GLfloat vertexPositions[] = {
//	0.75f, 0.75f, 0.0f, 1.0f,        1.0f, 0.0f, 0.0f, 1.0f,
//	0.75f, -0.75f, 0.0f, 1.0f,	     0.0f, 1.0f, 0.0f, 1.0f,
//	-0.75f, -0.75f, 0.0f, 1.0f,	     0.0f, 0.0f, 1.0f, 1.0f,
//
//};

std::vector<float> vertexData;

GLuint positionBufferObject = 0;
GLuint theProgram = 0;

GLuint CreateShader(GLenum eShaderType, const char *strShader)
{
	GLuint shader = glCreateShader(eShaderType);
	glShaderSource(shader, 1, &strShader, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Compile failure in %s shader:\n%s\n", (eShaderType == GL_VERTEX_SHADER ? "vertex" : (eShaderType == GL_GEOMETRY_SHADER ? "geometry" : "fragment")), strInfoLog);
		delete[] strInfoLog;
	}
	return shader;
}

GLuint CreateProgram(const vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();
	for (size_t i = 0; i < shaderList.size(); i++)
		glAttachShader(program, shaderList[i]);
	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	for (size_t i = 0; i < shaderList.size(); i++)
		glDetachShader(program, shaderList[i]);
	return program;
}

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_CULL_FACE);

	glUseProgram(theProgram);


	struct {
		glm::mat4 transform;
		glm::mat4 look;
		glm::mat4 proj;
	} tblock;

	tblock.transform = glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(1, 0, 0));
	tblock.transform = glm::rotate(tblock.transform, rot.y, glm::vec3(0, 1, 0));
	tblock.transform = glm::rotate(tblock.transform, rot.z, glm::vec3(0, 0, 1));
	tblock.transform = glm::translate(tblock.transform, trans);
	
	
	tblock.look = glm::lookAt(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
	tblock.proj = glm::perspective(60.0f, 1.0f, 0.0f, 20.0f);
	tblock.proj = glm::scale(tblock.proj, scale);

	GLuint blockIndex = glGetUniformBlockIndex(theProgram, "TBlock");
	GLint blockSize;
	glGetActiveUniformBlockiv(theProgram, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	GLubyte*blockBuffer = (GLubyte*)malloc(blockSize);
	const GLchar* names[] = { "transform", "look", "proj" }; 
	GLuint indices[3];
	glGetUniformIndices(theProgram, 3, names, indices);
	GLint offset[3];
	glGetActiveUniformsiv(theProgram, 3, indices, GL_UNIFORM_OFFSET, offset);
	memcpy(blockBuffer + offset[0], glm::value_ptr(tblock.transform), sizeof(tblock.transform));
	memcpy(blockBuffer + offset[1], glm::value_ptr(tblock.look), sizeof(tblock.look));
	memcpy(blockBuffer + offset[2], glm::value_ptr(tblock.proj), sizeof(tblock.proj));
	GLuint uBuf;
	glGenBuffers(1, &uBuf);
	glBindBuffer(GL_UNIFORM_BUFFER, uBuf);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uBuf);


	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (const GLvoid*)(sizeof(GLfloat) * 4));

	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() /2);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);

	glDisable(GL_CULL_FACE);

	glutSwapBuffers();
}

///////////////////////////////////////////////////////////
// File Reader
std::string readFile(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}


///////////////////////////////////////////////////////////
// Setup the rendering state
void SetupRC(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// setup vertex buffer
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// make shaders
	vector<GLuint> shaders;
	std::string vs = readFile("normalColor.v");
	std::string fs = readFile("normalColor.f");
	shaders.push_back(CreateShader(GL_VERTEX_SHADER, vs.c_str()));
	shaders.push_back(CreateShader(GL_FRAGMENT_SHADER, fs.c_str()));
	theProgram = CreateProgram(shaders);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void keyPress(unsigned char k, int x, int y)
{
	switch (k) {
	case 'x': rot.x += 0.1f; break;
	case 'y': rot.y += 0.1f; break;
	case 'z': rot.z += 0.1f; break;
	case 'X': rot.x -= 0.1f; break;
	case 'Y': rot.y -= 0.1f; break;
	case 'Z': rot.z -= 0.1f; break;
	case '+': scale.z += 0.001f; break;
	case '-': scale.z -= 0.001f; break;
	case 'w': trans.y += 0.1f;break;
	case 's': trans.y -= 0.1f;break;
	case 'd': trans.x += 0.1f;break;
	case 'a': trans.x -= 0.1f;break;
	case 'q': exit(1);
	}
	glutPostRedisplay();
}

void changeSize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

///////////////////////////////////////////////////////////
// Load Model
void loadModel(void) {
	loadOBJ("E:\\Datasets\\sphere.obj", vertexData);
}


///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char* argv[])
{
	loadModel();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Shader");

	glutKeyboardFunc(keyPress);
	glutReshapeFunc(changeSize);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 not supported\n");
		exit(1);
	}

	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
	return 0;
}

