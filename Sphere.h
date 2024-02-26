#ifndef SPHERE_H
#define SPHERE_H

#include "Shader.h"

class Sphere {
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	Shader shader;
	int indicesSize;

public:
	Sphere(float radius, float x, float y, float z, int stackCount, int sectorCount);
	void render(const glm::mat4& view, const glm::mat4& proj);
};

#endif
