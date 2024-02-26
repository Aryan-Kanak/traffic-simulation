#ifndef SHADER_H
#define SHADER_H

#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	unsigned int ID;
	mutable std::unordered_map<std::string, GLint> uniformLocationCache;

	GLint getUniformLocation(const std::string &name) const;

public:
	Shader(const char *vertex, const char *fragment);
	void use();
	void setUniformMatrix3fv(const std::string &name, const glm::mat3 &matrix) const;
	void setUniformMatrix4fv(const std::string &name, const glm::mat4 &matrix) const;
	void setUniform1f(const std::string &name, float value) const;
	void setUniform3f(const std::string &name, float v0, float v1, float v2) const;
	void setUniform1i(const std::string &name, int value);
};

#endif // !SHADER_H
