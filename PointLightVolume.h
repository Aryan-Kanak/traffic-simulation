#ifndef SPHERE_H
#define SPHERE_H

#include "Shader.h"

struct PointLight {
	float constant;
	float linear;
	float quadratic;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float CalcRadius() {
		float lightMax = std::fmaxf(std::fmaxf(diffuse.r, diffuse.g), diffuse.b);
		return (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
			/ (2 * quadratic);
	}
};

class PointLightVolume {
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	Shader shader;
	int indicesSize;
	glm::mat4 model;

public:
	PointLightVolume(PointLight &light, int stackCount = 18, int sectorCount = 36);
	void render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos, const glm::vec3& lightPos);
};

#endif
