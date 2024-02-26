#include <iostream> // FOR DEBUGGING
#include <string>

#include "Road.h"
#include "Moonlight.h"

const float Road::spawnPeriod = 5.0f;

Road::Road(int x1, int y1, int x2, int y2) : shader{Shader("lighting.vert", "lighting.frag")},
gBufferShader{Shader("lighting.vert", "gBuffer.frag")}, model{glm::mat4(1.0f)},
x1{ x1 }, y1{ y1 }, x2{ x2 }, y2{ y2 }, timeSinceLastSpawn{ 0.0f }
{
	length = glm::length(glm::vec2(x2 - x1, y2 - y1));

	glm::vec3 direction = glm::vec3(x2 - x1, y2 - y1, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 ortho = glm::normalize(glm::cross(direction, up));
	ortho = 0.5f * ortho;

	float vertices[] = {
		x1 - ortho.x, -4.0f, y1 - ortho.y, 0.0f, 1.0f, 0.0f,
		x2 - ortho.x, -4.0f, y2 - ortho.y, 0.0f, 1.0f, 0.0f,
		x2 + ortho.x, -4.0f, y2 + ortho.y, 0.0f, 1.0f, 0.0f,
		x1 + ortho.x, -4.0f, y1 + ortho.y, 0.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	vertexData = VertexData(vertices, sizeof(vertices), indices, sizeof(indices));
}

Road::~Road()
{
	for (int i = 0; i < cars.size(); ++i) {
		delete cars[i];
	}
}

void Road::update(float deltaTime)
{
	// spawn new cars
	timeSinceLastSpawn += deltaTime;
	if (timeSinceLastSpawn >= spawnPeriod) {
		timeSinceLastSpawn -= spawnPeriod;
		//spawnCar(0.0f);
	}

	// update traffic lights
	for (int i = 0; i < trafficLights.size(); ++i) {
		trafficLights[i]->update(deltaTime);
	}

	for (int i = 0; i < cars.size(); ++i) {
		if (cars[i]->getPosition() > length) {
			std::cout << cars.size() << " " << i << std::endl;
		}
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

			if (curLight != nullptr && curLight->state != green) {
				if (position + cars[i]->getLength() / 2 >= curLight->position - curLight->getStopDistance()) {
					// stop car
					cars[i]->Stop();
				} else if (position + cars[i]->getLength() / 2 >= curLight->position - curLight->getSlowDistance()) {
					// slow down car
					cars[i]->Slow();
				}
			} else {
				cars[i]->Unslow();
				cars[i]->Unstop();
			}
		}
	}

	// remove car once it's out of bounds
	if (cars.size() > 0 && cars[cars.size() - 1]->getPosition() > length) {
		delete cars[cars.size() - 1];
		cars.pop_back();
	}
}

void Road::render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3 &cameraPos, const glm::vec3 *lightPositions)
{
	shader.use();

	shader.setUniformMatrix4fv("model", model);
	shader.setUniformMatrix4fv("projection", proj);
	shader.setUniformMatrix4fv("view", view);
	glm::mat3 normMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
	shader.setUniformMatrix3fv("normMatrix", normMatrix);

	shader.setUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	// set material properties
	shader.setUniform3f("material.ambient", 0.0f, 0.0f, 0.0f);
	shader.setUniform3f("material.diffuse", 0.51f, 0.49f, 0.47f);
	shader.setUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
	shader.setUniform1f("material.shininess", 4.0f);

	// set moonlight properties
	Moonlight::SetMoonlightUniforms(shader);

	// set point light properties
	for (int i = 0; i < 1; ++i) {
		std::string uniformName = "pointLights[" + std::to_string(i) + "].position";
		shader.setUniform3f(uniformName, lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
		uniformName = "pointLights[" + std::to_string(i) + "].constant";
		shader.setUniform1f(uniformName, 1.0f);
		uniformName = "pointLights[" + std::to_string(i) + "].linear";
		shader.setUniform1f(uniformName, 0.7f);
		uniformName = "pointLights[" + std::to_string(i) + "].quadratic";
		shader.setUniform1f(uniformName, 1.8f);
		if (i % 4 == 0 || i % 4 == 1) {
			// headlight
			uniformName = "pointLights[" + std::to_string(i) + "].ambient";
			shader.setUniform3f(uniformName, 0.2f, 0.2f, 0.2f);
			uniformName = "pointLights[" + std::to_string(i) + "].diffuse";
			shader.setUniform3f(uniformName, 0.5f, 0.5f, 0.5f);
			uniformName = "pointLights[" + std::to_string(i) + "].specular";
			shader.setUniform3f(uniformName, 1.0f, 1.0f, 1.0f);
		}
		else {
			// taillight
			uniformName = "pointLights[" + std::to_string(i) + "].ambient";
			shader.setUniform3f(uniformName, 0.2f, 0.0f, 0.0f);
			uniformName = "pointLights[" + std::to_string(i) + "].diffuse";
			shader.setUniform3f(uniformName, 0.5f, 0.0f, 0.0f);
			uniformName = "pointLights[" + std::to_string(i) + "].specular";
			shader.setUniform3f(uniformName, 1.0f, 0.0f, 0.0f);
		}
	}

	glBindVertexArray(vertexData.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	renderCars(view, proj, cameraPos);
	renderTrafficLights(view, proj);
}

void Road::fillGBuffer(const glm::mat4& view, const glm::mat4& proj)
{
	gBufferShader.use();

	gBufferShader.setUniformMatrix4fv("model", model);
	gBufferShader.setUniformMatrix4fv("projection", proj);
	gBufferShader.setUniformMatrix4fv("view", view);
	glm::mat3 normMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
	gBufferShader.setUniformMatrix3fv("normMatrix", normMatrix);

	// set material properties
	gBufferShader.setUniform3f("material.ambient", 0.0f, 0.0f, 0.0f);
	gBufferShader.setUniform3f("material.diffuse", 0.51f, 0.49f, 0.47f);
	gBufferShader.setUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
	gBufferShader.setUniform1f("material.shininess", 4.0f);

	glBindVertexArray(vertexData.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	for (int i = 0; i < cars.size(); ++i) {
		cars[i]->fillGBuffer(view, proj);
	}
	/*for (int i = 0; i < trafficLights.size(); ++i) {
		trafficLights[i]->fillGBuffer(view, proj);
	}*/
}

void Road::forwardRender(const glm::mat4& view, const glm::mat4& proj)
{
	for (int i = 0; i < cars.size(); ++i) {
		cars[i]->forwardRender(view, proj);
	}
	for (int i = 0; i < trafficLights.size(); ++i) {
		trafficLights[i]->forwardRender(view, proj);
	}
}

void Road::renderCars(const glm::mat4& view, const glm::mat4& proj, const glm::vec3 &cameraPos)
{
	for (int i = 0; i < cars.size(); ++i) {
		cars[i]->render(view, proj, cameraPos);
	}
}

void Road::renderTrafficLights(const glm::mat4& view, const glm::mat4& proj)
{
	for (int i = 0; i < trafficLights.size(); ++i) {
		trafficLights[i]->render(view, proj);
	}
}

void Road::spawnCar(float position)
{
	cars.push_front(new Car(this, position));
	nextLight.push_front(0);
}

void Road::addTrafficLight(float position, State startState)
{
	trafficLights.push_back(new TrafficLight(this, position, startState));
}

glm::vec3* Road::getLightPositions() const
{
	glm::vec3 *lightPositions = new glm::vec3[4 * cars.size()];
	for (int i = 0; i < cars.size(); ++i) {
		glm::vec3 *curPositions = cars[i]->getLightPositions();
		for (int j = 0; j < 4; ++j) {
			lightPositions[4 * i + j] = curPositions[j];
		}
		delete[] curPositions;
	}

	return lightPositions;
}

float Road::GetX1() const
{
	return x1;
}

float Road::GetX2() const
{
	return x2;
}

float Road::GetY1() const
{
	return y1;
}

float Road::GetY2() const
{
	return y2;
}
