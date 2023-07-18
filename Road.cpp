#include "Road.h"

#include <iostream> // FOR DEBUGGING


Road::Road(int x1, int y1, int x2, int y2) : shader{Shader("vertex.glsl", "fragment.glsl")}, model{glm::mat4(1.0f)},
											x1{ x1 }, y1{ y1 }, x2{ x2 }, y2{ y2 }
{
	length = glm::length(glm::vec2(x2 - x1, y2 - y1));

	glm::vec3 direction = glm::vec3(x2 - x1, y2 - y1, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 ortho = glm::normalize(glm::cross(direction, up));
	ortho = 0.5f * ortho;

	float vertices[] = {
		x1 - ortho.x, -4.0f, y1 - ortho.y, 0.51f, 0.49f, 0.47f,
		x2 - ortho.x, -4.0f, y2 - ortho.y, 0.51f, 0.49f, 0.47f,
		x2 + ortho.x, -4.0f, y2 + ortho.y, 0.51f, 0.49f, 0.47f,
		x1 + ortho.x, -4.0f, y1 + ortho.y, 0.51f, 0.49f, 0.47f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

Road::~Road()
{
	for (int i = 0; i < cars.size(); ++i) {
		delete cars[i];
	}
}

void Road::update()
{
	for (int i = 0; i < cars.size(); ++i) {
		// update position of car
		Car* next = nullptr;
		if (i < cars.size() - 1) {
			next = cars[i + 1];
		}
		cars[i]->updatePosition(next);

		// check if car has reached traffic light
		if (nextLight[i] < trafficLights.size()) {
			TrafficLight* curLight = trafficLights[nextLight[i]];
			float position = cars[i]->getPosition();
			if (curLight->position < position) {
				nextLight[i]++;
				curLight = nextLight[i] < trafficLights.size() ? trafficLights[nextLight[i]] : nullptr;
				// unslow and unstop car
				cars[i]->Unslow();
				cars[i]->Unstop();
			}

			if (curLight != nullptr && curLight->state == red) {
				if (position + cars[i]->getLength() / 2 >= curLight->position - curLight->getStopDistance()) {
					// stop car
					cars[i]->Stop();
				} else if (position + cars[i]->getLength() / 2 >= curLight->position - curLight->getSlowDistance()) {
					// slow down car
					cars[i]->Slow();
				}
			}
		}
	}

	// remove car once it's out of bounds
	if (cars.size() > 0 && cars[cars.size() - 1]->getPosition() > length) {
		delete cars[cars.size() - 1];
		cars.pop_back();
	}
}

void Road::render(const glm::mat4& view, const glm::mat4& proj)
{
	shader.use();

	shader.setUniformMatrix4fv("model", model);
	shader.setUniformMatrix4fv("projection", proj);
	shader.setUniformMatrix4fv("view", view);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	renderCars(view, proj);
	renderTrafficLights(view, proj);
}

void Road::renderCars(const glm::mat4& view, const glm::mat4& proj)
{
	for (int i = 0; i < cars.size(); ++i) {
		cars[i]->render(view, proj);
	}
}

void Road::renderTrafficLights(const glm::mat4& view, const glm::mat4& proj)
{
	for (int i = 0; i < trafficLights.size(); ++i) {
		trafficLights[i]->render(view, proj);
	}
}

void Road::spawnCar()
{
	cars.push_back(new Car(this));
	nextLight.push_back(0);
}

void Road::addTrafficLight(float position)
{
	trafficLights.push_back(new TrafficLight(this, position));
}

const float Road::GetX1()
{
	return x1;
}

const float Road::GetX2()
{
	return x2;
}

const float Road::GetY1()
{
	return y1;
}

const float Road::GetY2()
{
	return y2;
}
