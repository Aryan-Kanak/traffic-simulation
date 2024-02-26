#ifndef VEHICLE_H
#define VEHICLE_H

#include "Shader.h"

class Road;

class Vehicle {
protected:
	float height;
	float length;
	float width;
	static const float globalMaxVelocity;
	static const float slowVelocity;

	float position;
	float velocity;
	float acceleration;
	float safeDist;
	float reactionTime;
	float maxVelocity;
	float maxAcceleration;
	float deceleration;
	float smoothness;
	bool stopping;
	Road *road;

	unsigned int body_vao;
	unsigned int body_vbo;
	unsigned int body_ebo;
	unsigned int lights_vao;
	unsigned int lights_vbo;
	unsigned int lights_ebo;
	glm::mat4 model;
	Shader shader;
public:
	virtual void updatePosition(Vehicle *lead) = 0;
	virtual void updateModelMatrix() = 0;
	virtual void render(const glm::mat4& view, const glm::mat4& proj) = 0;
	void Stop();
	void Unstop();
	void Slow();
	void Unslow();
	const float getHeight();
	const float getLength();
	const float getWidth();
	const float getPosition();
	const float getVelocity();
	const float getAcceleration();
};

#endif // !VEHICLE_H
