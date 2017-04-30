#pragma once
#include <vector>

class Vertex
{
public:
	float x, y, z, w = 1.0f;
};

bool loadOBJ(const char * path, std::vector<Vertex> & out_vertices);

void RenderSceneModel(void);

void ModelChangeView(GLsizei w, GLsizei h);

void ModelKeyboard(unsigned char c, int i0, int i1);
