#include "Shader.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

GLint Shader::getUniformLocation(const std::string &name) const
{
	auto it = uniformLocationCache.find(name);
	if (it != uniformLocationCache.end()) {
		return it->second;
	}

	GLint loc = glGetUniformLocation(this->ID, name.c_str());
	if (loc == -1) {
		std::cout << "ERROR: No uniform with name " << name << std::endl;
	}
	uniformLocationCache[name] = loc;
	return loc;
}

Shader::Shader(const char *vertex, const char *fragment)
{
	// retrieve shader code from files
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::stringstream vertexSS;
	std::stringstream fragmentSS;
	
	vertexFile.open(vertex);
	fragmentFile.open(fragment);
	vertexSS << vertexFile.rdbuf();
	fragmentSS << fragmentFile.rdbuf();
	vertexFile.close();
	fragmentFile.close();
	std::string tempVertexCode = vertexSS.str();
	std::string tempFragmentCode = fragmentSS.str();
	const char *vertexCode = tempVertexCode.c_str();
	const char *fragmentCode = tempFragmentCode.c_str();

	// compile shaders
	int success;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED" << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED" << std::endl;
	}

	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		std::cout << "ERROR: SHADER PROGRAM LINKING FAILED" << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setUniformMatrix3fv(const std::string &name, const glm::mat3 &matrix) const
{
	GLint loc = glGetUniformLocation(this->ID, name.c_str());
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformMatrix4fv(const std::string &name, const glm::mat4 &matrix) const
{
	GLint loc = glGetUniformLocation(this->ID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform1f(const std::string &name, float value) const
{
	GLint loc = glGetUniformLocation(this->ID, name.c_str());
	glUniform1f(loc, value);
}

void Shader::setUniform3f(const std::string &name, float v0, float v1, float v2) const
{
	GLint loc = glGetUniformLocation(this->ID, name.c_str());
	glUniform3f(loc, v0, v1, v2);
}

void Shader::setUniform1i(const std::string& name, int value)
{
	GLint loc = glGetUniformLocation(this->ID, name.c_str());
	glUniform1i(loc, value);
}
