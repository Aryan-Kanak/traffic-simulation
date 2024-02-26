#ifndef CAR_H
#define CAR_H

#include "Shader.h"
#include "VertexData.h"

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

	VertexData bodyVertexData;
	VertexData lightsVertexData;
	glm::mat4 model;
	Shader bodyShader;
	Shader lightsShader;
	Shader gBufferShader;
public:
	Car(Road *road, float position);
	void updatePosition(Car *lead);
	void updateModelMatrix();
	void render(const glm::mat4 &view, const glm::mat4 &proj, const glm::vec3 &cameraPos);
	void fillGBuffer(const glm::mat4 &view, const glm::mat4 &proj);
	void forwardRender(const glm::mat4 &view, const glm::mat4 &proj);
	void Stop();
	void Unstop();
	void Slow();
	void Unslow();
	// returns a dynamic array of size 4 (needs to be freed by caller)
	glm::vec3 *getLightPositions() const;
	float getHeight() const;
	float getLength() const;
	float getWidth() const;
	float getPosition() const;
	float getVelocity() const;
	float getAcceleration() const;
};

#endif // !CAR_H

