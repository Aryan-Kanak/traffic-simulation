#ifndef ROAD_H
#define ROAD_H

#include <vector>
#include <deque>

#include "Shader.h"
#include "Car.h"
#include "TrafficLight.h"

class Road {
private:
	int x1;
	int y1;
	int x2;
	int y2;
	int length;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	glm::mat4 model;
	Shader shader;
	std::deque<Car *> cars;
	std::deque<float> nextLight;
	std::vector<TrafficLight *> trafficLights; // sorted by position
public:
	Road(int x1, int y1, int x2, int y2);
	~Road();
	void update();
	void render(const glm::mat4& view, const glm::mat4& proj);
	void renderCars(const glm::mat4& view, const glm::mat4& proj);
	void renderTrafficLights(const glm::mat4& view, const glm::mat4& proj);
	void spawnCar();
	void addTrafficLight(float position);
	const float GetX1();
	const float GetX2();
	const float GetY1();
	const float GetY2();
};

#endif // !ROAD_H
