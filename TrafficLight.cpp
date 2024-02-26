#include "TrafficLight.h"
#include "Road.h"

// should be of the form [g, y, r = g + y]
const float TrafficLight::lightDuration[3] = { 5.0f, 2.0f, 7.0f };

TrafficLight::TrafficLight(Road* road, float position, State startState) : bodyShader{Shader("default.vert", "default.frag")},
lightShader{Shader("default.vert", "manualColour.frag")},
gBufferShader{Shader("lighting.vert", "gBuffer.frag")},
road{ road }, position{ position }, state{ startState }, slowDistance{ 10.0f }, stopDistance{ 2.5f }, timeSinceLastUpdate{ 0.0f }
{
	float bodyVertices[] = {
		-0.05f, 0.15f, 0.05f, 0.18f, 0.18f, 0.18f,
		0.05f, 0.15f, 0.05f, 0.18f, 0.18f, 0.18f,
		0.05f, -0.15f, 0.05f, 0.18f, 0.18f, 0.18f,
		-0.05f, -0.15f, 0.05f, 0.18f, 0.18f, 0.18f,
		-0.05f, 0.15f, -0.05f, 0.18f, 0.18f, 0.18f,
		0.05f, 0.15f, -0.05f, 0.18f, 0.18f, 0.18f,
		0.05f, -0.15f, -0.05f, 0.18f, 0.18f, 0.18f,
		-0.05f, -0.15f, -0.05f, 0.18f, 0.18f, 0.18f
	};

	unsigned int bodyIndices[] = {
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

	float redLightVertices[] = {
		-0.05f, 0.15f, 0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, 0.15f, 0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, 0.05f, 0.05f, 0.0f, 0.0f, 0.0f,
		-0.05f, 0.05f, 0.05f, 0.0f, 0.0f, 0.0f,
	};

	float yellowLightVertices[] = {
		-0.05f, 0.05f, 0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, 0.05f, 0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, -0.05f, 0.05f, 0.0f, 0.0f, 0.0f,
		-0.05f, -0.05f, 0.05f, 0.0f, 0.0f, 0.0f,
	};

	float greenLightVertices[] = {
		-0.05f, -0.05f, 0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, -0.05f, 0.05f, 0.0f, 0.0f, 0.0f,
		0.05f, -0.15f, 0.05f, 0.0f, 0.0f, 0.0f,
		-0.05f, -0.15f, 0.05f, 0.0f, 0.0f, 0.0f,
	};

	unsigned int lightIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	bodyVertexData = VertexData(bodyVertices, sizeof(bodyVertices), bodyIndices, sizeof(bodyIndices));
	redLightVertexData = VertexData(redLightVertices, sizeof(redLightVertices), lightIndices, sizeof(lightIndices));
	yellowLightVertexData = VertexData(yellowLightVertices, sizeof(yellowLightVertices), lightIndices, sizeof(lightIndices));
	greenLightVertexData = VertexData(greenLightVertices, sizeof(greenLightVertices), lightIndices, sizeof(lightIndices));

	// set up model matrix
	float x1 = road->GetX1();
	float x2 = road->GetX2();
	float y1 = road->GetY1();
	float y2 = road->GetY2();
	glm::vec3 roadDirection = glm::normalize(glm::vec3(x2 - x1, 0.0f, y2 - y1));
	glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	
	glm::vec2 increment = glm::vec2(roadDirection.x, roadDirection.z) * (float) position;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x1 + increment.x, -2.0f, y1 + increment.y));

	float dotProduct = glm::dot(lightDirection, roadDirection);
	float rotationAngle = acos(dotProduct);
	glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	if (glm::abs(dotProduct) != 1) {
		rotationAxis = glm::cross(lightDirection, roadDirection);
	}
	model = glm::rotate(model, rotationAngle, rotationAxis);
}

void TrafficLight::render(const glm::mat4& view, const glm::mat4& proj)
{
	// render body
	bodyShader.use();

	bodyShader.setUniformMatrix4fv("model", model);
	bodyShader.setUniformMatrix4fv("projection", proj);
	bodyShader.setUniformMatrix4fv("view", view);

	glBindVertexArray(bodyVertexData.vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// render lights
	lightShader.use();

	lightShader.setUniformMatrix4fv("model", model);
	lightShader.setUniformMatrix4fv("projection", proj);
	lightShader.setUniformMatrix4fv("view", view);

	const float dimFactor = 0.25f;
	glm::vec3 colourRed(1.0f, 0.0f, 0.0f);
	if (state != red) {
		colourRed = dimFactor * colourRed;
	}
	lightShader.setUniform3f("colour", colourRed.r, colourRed.g, colourRed.b);
	glBindVertexArray(redLightVertexData.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glm::vec3 colourYellow(1.0f, 0.9f, 0.0f);
	if (state != yellow) {
		colourYellow = dimFactor * colourYellow;
	}
	lightShader.setUniform3f("colour", colourYellow.r, colourYellow.g, colourYellow.b);
	glBindVertexArray(yellowLightVertexData.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glm::vec3 colourGreen(0.0f, 1.0f, 0.0f);
	if (state != green) {
		colourGreen = dimFactor * colourGreen;
	}
	lightShader.setUniform3f("colour", colourGreen.r, colourGreen.g, colourGreen.b);
	glBindVertexArray(greenLightVertexData.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TrafficLight::forwardRender(const glm::mat4& view, const glm::mat4& proj)
{
	this->render(view, proj);
}

void TrafficLight::update(float deltaTime)
{
	timeSinceLastUpdate += deltaTime;
	if (timeSinceLastUpdate >= lightDuration[state]) {
		timeSinceLastUpdate -= lightDuration[state];
		if (state == red) {
			state = green;
		} else if (state == yellow) {
			state = red;
		} else if (state == green) {
			state = yellow;
		}
	}
}

const float TrafficLight::getSlowDistance()
{
	return slowDistance;
}

const float TrafficLight::getStopDistance()
{
	return stopDistance;
}
