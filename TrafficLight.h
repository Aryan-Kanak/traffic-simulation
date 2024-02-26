#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "VertexData.h"
#include "Shader.h"

class Road;

enum State {green, yellow, red};

class TrafficLight {
private:
	static const float lightDuration[3];

	Road *road;
	const float slowDistance;
	const float stopDistance;
	float timeSinceLastUpdate;

	VertexData bodyVertexData;
	VertexData redLightVertexData;
	VertexData yellowLightVertexData;
	VertexData greenLightVertexData;

	glm::mat4 model;
	Shader bodyShader;
	Shader lightShader;
	Shader gBufferShader;
public:
	State state;
	float position;

	TrafficLight(Road* road, float position, State startState);
	void render(const glm::mat4& view, const glm::mat4& proj);
	void fillGBuffer(const glm::mat4 &view, const glm::mat4 &proj);
	void forwardRender(const glm::mat4& view, const glm::mat4& proj);
	void update(float deltaTime);
	const float getSlowDistance();
	const float getStopDistance();
};

#endif // !TRAFFIC_LIGHT_H
