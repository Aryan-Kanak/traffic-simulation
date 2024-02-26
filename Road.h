#ifndef ROAD_H
#define ROAD_H

#include <vector>
#include <deque>

#include "VertexData.h"
#include "Shader.h"
#include "Car.h"
#include "TrafficLight.h"

class Road {
private:
	static const float spawnPeriod;

	int x1;
	int y1;
	int x2;
	int y2;
	int length;

	VertexData vertexData;
	glm::mat4 model;
	Shader shader;
	Shader gBufferShader;

	std::deque<Car *> cars;
	std::deque<float> nextLight;
	std::vector<TrafficLight *> trafficLights; // sorted by position
	float timeSinceLastSpawn;
public:
	Road(int x1, int y1, int x2, int y2);
	~Road();
	void update(float deltaTime);
	void render(const glm::mat4 &view, const glm::mat4 &proj, const glm::vec3 &cameraPos, const glm::vec3 *lightPositions);
	void fillGBuffer(const glm::mat4 &view, const glm::mat4 &proj);
	void forwardRender(const glm::mat4& view, const glm::mat4& proj);
	void renderCars(const glm::mat4 &view, const glm::mat4 &proj, const glm::vec3 &cameraPos);
	void renderTrafficLights(const glm::mat4 &view, const glm::mat4 &proj);
	void spawnCar(float position);
	void addTrafficLight(float position, State startState);
	// returns a dynamic array (needs to be freed by caller)
	glm::vec3 *getLightPositions() const;
	float GetX1() const;
	float GetX2() const;
	float GetY1() const;
	float GetY2() const;
};

#endif // !ROAD_H
