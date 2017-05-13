#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

layout(std140) uniform TBlock{
	mat4 transform;
	mat4 look;
	mat4 proj;
};

out vec3 localNormal;
out vec3 lightDir;
void main() {
	lightDir = normalize(vec3(1.0, 1.0, 1.0));
	localNormal = normalize(normal);
    gl_Position = proj * look * transform * position;
}