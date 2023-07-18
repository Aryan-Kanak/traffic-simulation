#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	const float cameraSpeed;
	bool firstMouse;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
public:
	Camera(int window_width, int window_height);
	void moveForward(float deltaTime);
	void moveBackward(float deltaTime);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
	void handleMouseInput(double xpos, double ypos);
	glm::mat4 getViewMatrix() const;
};

#endif // !CAMERA_H
