#ifndef CAR_H
#define CAR_H

#include "Shader.h"


class Road;

class Car {
private:
	static const float height;
	static const float length;
	static const float width;
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
	Car(Road *road);
	void updatePosition(Car *lead);
	void updateModelMatrix();
	void render(const glm::mat4& view, const glm::mat4& proj);
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

#endif // !CAR_H

