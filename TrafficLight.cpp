#include "TrafficLight.h"
#include "Road.h"

TrafficLight::TrafficLight(Road* road, float position) : shader{Shader("vertex.glsl", "fragment.glsl")},
road{ road }, position{ position }, state{ red }, slowDistance{ 10.0f }, stopDistance{ 2.5f }
{
	float vertices[] = {
		-0.05f, 0.15f, 0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, 0.15f, 0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, -0.15f, 0.05f, 0.0f, 0.0f, 0.0f,
		-0.05f, -0.15f, 0.05f, 0.0f, 0.0f, 0.0f,
		-0.05f, 0.15f, -0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, 0.15f, -0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, -0.15f, -0.05f, 0.0f, 0.0f, 0.0f,
		-0.05f, -0.15f, -0.05f, 0.0f, 0.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 3, 2,
		1, 5, 6,
		1, 6, 2,
		4, 5, 6,
		4, 6, 7,
		0, 4, 7,
		0, 3, 7,
		4, 5, 1,
		4, 0, 1,
		7, 2, 3,
		7, 6, 2
	};

	glGenVertexArrays(1, &body_vao);
	glBindVertexArray(body_vao);

	glGenBuffers(1, &body_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, body_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &body_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, body_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// set up model matrix
	float x1 = road->GetX1();
	float x2 = road->GetX2();
	float y1 = road->GetY1();
	float y2 = road->GetY2();
	glm::vec2 roadDirection = glm::normalize(glm::vec2(x2 -x1, y2 - y1));
	glm::vec2 increment = roadDirection * (float) position;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3((x1 + x2) / 2 + increment.x, -2.0f , y1 + increment.y));
}

void TrafficLight::render(const glm::mat4& view, const glm::mat4& proj)
{
	shader.use();

	shader.setUniformMatrix4fv("model", model);
	shader.setUniformMatrix4fv("projection", proj);
	shader.setUniformMatrix4fv("view", view);

	glBindVertexArray(body_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

const float TrafficLight::getSlowDistance()
{
	return slowDistance;
}

const float TrafficLight::getStopDistance()
{
	return stopDistance;
}
