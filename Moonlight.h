#ifndef MOONLIGHT_H
#define MOONLIGHT_H

#include "Shader.h"

#include <glm/glm.hpp>

namespace Moonlight {
	namespace {
		const glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		const glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		const glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		const glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	void SetMoonlightUniforms(const Shader& shader);
}

#endif // !MOONLIGHT_H
