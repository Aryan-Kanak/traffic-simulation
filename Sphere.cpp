#include <vector>

#include <glm/glm.hpp>

#include "Sphere.h"

// taken from https://www.songho.ca/opengl/gl_sphere.html
Sphere::Sphere(float radius, float x, float y, float z, int stackCount, int sectorCount): shader{Shader("lightVolume.vert", "lightVolume.frag")}
{
	std::vector<float> vertices;
	std::vector<float> indices;

	const float PI = glm::pi<float>();

	float sectorStep = 2 * PI;
	float stackStep = PI / stackCount;

	for (int i = 0; i <= stackCount; ++i) {
		float stackAngle = PI / 2 - i * stackStep;
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);
		
		for (int j = 0; j <= sectorCount; ++j) {
			float sectorAngle = j * sectorStep;

			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	for (int i = 0; i < stackCount; ++i) {
		int k1 = i * (sectorCount + 1);
		int k2 = k1 + sectorCount + 1;
		
		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != stackCount - 1) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	int verticesSize = vertices.size();
	indicesSize = indices.size();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Sphere::render(const glm::mat4& view, const glm::mat4& proj)
{
	shader.use();

	glm::mat4 model = glm::mat4(1.0f);
	shader.setUniformMatrix4fv("model", model);
	shader.setUniformMatrix4fv("projection", proj);
	shader.setUniformMatrix4fv("view", view);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}
