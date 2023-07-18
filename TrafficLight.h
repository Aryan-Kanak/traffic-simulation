#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "Shader.h"

class Road;

enum State {red, yellow, green};

class TrafficLight {
private:
	Road *road;
	const float slowDistance;
	const float stopDistance;
	unsigned int body_vao;
	unsigned int body_vbo;
	unsigned int body_ebo;
	glm::mat4 model;
	Shader shader;
public:
	State state;
	float position;

	TrafficLight(Road* road, float position);
	void render(const glm::mat4& view, const glm::mat4& proj);
	const float getSlowDistance();
	const float getStopDistance();
};

#endif // !TRAFFIC_LIGHT_H
