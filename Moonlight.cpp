#include "Moonlight.h"

void Moonlight::SetMoonlightUniforms(const Shader& shader)
{
	shader.setUniform3f("moonlight.direction", Moonlight::direction.x, Moonlight::direction.y, Moonlight::direction.z);
	shader.setUniform3f("moonlight.ambient", Moonlight::ambient.x, Moonlight::ambient.y, Moonlight::ambient.z);
	shader.setUniform3f("moonlight.diffuse", Moonlight::diffuse.x, Moonlight::diffuse.y, Moonlight::diffuse.z);
	shader.setUniform3f("moonlight.specular", Moonlight::specular.x, Moonlight::specular.y, Moonlight::specular.z);
}
