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

#include <math.h>
#include <string>
#include <vector>

using namespace std;

GLfloat rotx = 0;
GLfloat roty = 0;

glm::vec3 rot(0.0f);
glm::vec3 trans(1.0f);

const GLfloat vertexPositions[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
};

GLuint positionBufferObject = 0;
GLuint theProgram = 0;

const char vs1[] = R"EOF(
#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 transform;
smooth out vec4 myColor;
void main() {
    gl_Position = transform * position;
	myColor = color;
}
)EOF";

const char fs1[] = R"EOF(
#version 330
smooth in vec4 myColor;
out vec4 outColor;
void main(){
   //outColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	outColor = myColor;
}
)EOF";


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
    for(size_t i = 0; i < shaderList.size(); i++)
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
    for(size_t i = 0; i < shaderList.size(); i++)
        glDetachShader(program, shaderList[i]);
    return program;
}

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(theProgram);

	glm::mat4 rx = glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(1, 0, 0));
	glm::mat4 ry = glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(0, 1, 0));
	glm::mat4 rz = glm::rotate(glm::mat4(1.0f), rot.z, glm::vec3(0, 0, 1));
	glm::mat4 t = glm::scale(glm::mat4(1.0f), glm::vec3(trans.z)); //glm::translate(glm::mat4(1.0f), glm::vec3(trans.z));
	
	glm::mat4 m = rx*ry*rz*t;
	glUniformMatrix4fv(
		glGetUniformLocation(theProgram, "transform"),
		1, GL_FALSE, glm::value_ptr(m));


	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(3 * 4 * 4));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glUseProgram(0);

    glutSwapBuffers();
}

///////////////////////////////////////////////////////////
// Setup the rendering state
void SetupRC(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// setup vertex buffer
    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// make shaders
	vector<GLuint> shaders;
	shaders.push_back(CreateShader(GL_VERTEX_SHADER, vs1));
	shaders.push_back(CreateShader(GL_FRAGMENT_SHADER, fs1));
	theProgram = CreateProgram(shaders);
}

void keyPress(unsigned char k, int x, int y)
{
	switch(k) {
		case 'x': rot.x +=0.1; break;
		case 'y': rot.y +=0.1; break;
		case 'z': rot.z +=0.1; break;
		case 'X': rot.x -=0.1; break;
		case 'Y': rot.y -=0.1; break;
		case 'Z': rot.z -=0.1; break;
		case '+': trans.z +=0.1f; break;
		case '-': trans.z -=0.1f; break;
		case 'q': exit(1);
	}
	glutPostRedisplay();
}

void changeSize(int w, int h) 
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE  | GLUT_RGBA | GLUT_DEPTH );

	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow("Shader");

	glutKeyboardFunc(keyPress);
	glutReshapeFunc(changeSize);

	GLenum err = glewInit();
	if (GLEW_OK != err)	{
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

