#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Skybox {
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int textureID;
	Shader shader;

	unsigned int loadCubemap(std::vector<std::string>& faces);

public:
	Skybox(std::vector<std::string> &faces);
	void render(glm::mat4 &view, glm::mat4 &projection);
};

#endif
