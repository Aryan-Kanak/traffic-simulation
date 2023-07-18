#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	unsigned int ID;

public:
	Shader(const char *vertex, const char *fragment);
	void use();
	void setUniformMatrix4fv(const char *name, const glm::mat4 &matrix) const;
};

#endif // !SHADER_H
