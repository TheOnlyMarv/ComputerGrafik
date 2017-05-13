#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

layout(std140) uniform TBlock{
	mat4 transform;
	mat4 look;
	mat4 proj;
};

smooth out vec4 myColor;
void main() {
    gl_Position = proj * look * transform * position;
	myColor = vec4(normal,1.0);
}