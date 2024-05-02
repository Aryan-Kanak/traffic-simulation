#include <vector>

#include <glm/glm.hpp>

#include "PointLightVolume.h"

// taken from https://www.songho.ca/opengl/gl_sphere.html
PointLightVolume::PointLightVolume(PointLight &light, int stackCount, int sectorCount):
	shader{Shader("lightVolume.vert", "lightVolume.frag")}, model{glm::mat4(1.0f)}
{
	shader.use();
	shader.setUniform1i("gPosition", 0);
	shader.setUniform1i("gNormal", 1);
	shader.setUniform1i("gAlbedo", 2);
	shader.setUniform1i("gSpecular", 3);
	shader.setUniform1f("light.constant", light.constant);
	shader.setUniform1f("light.linear", light.linear);
	shader.setUniform1f("light.quadratic", light.quadratic);
	shader.setUniform3f("light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
	shader.setUniform3f("light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
	shader.setUniform3f("light.specular", light.specular.x, light.specular.y, light.specular.z);

	float radius = light.CalcRadius();

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	const float PI = glm::pi<float>();

	float sectorStep = 2 * PI / sectorCount;
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
		unsigned int k1 = i * (sectorCount + 1);
		unsigned int k2 = k1 + sectorCount + 1;
		
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
	glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void PointLightVolume::render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos, const glm::vec3& lightPos)
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(lightPos.x, lightPos.y, lightPos.z));

	shader.use();
	shader.setUniformMatrix4fv("model", model);
	shader.setUniformMatrix4fv("projection", proj);
	shader.setUniformMatrix4fv("view", view);
	shader.setUniform3f("viewPos", viewPos.x, viewPos.y, viewPos.z);
	shader.setUniform3f("light.position", lightPos.x, lightPos.y, lightPos.z);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}
