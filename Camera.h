#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	static const float cameraSpeed;
	static const float sprintSpeed;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	bool firstMouse;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
public:
	Camera(int window_width, int window_height);
	void moveForward(float deltaTime, bool isSprinting);
	void moveBackward(float deltaTime, bool isSprinting);
	void moveLeft(float deltaTime, bool isSprinting);
	void moveRight(float deltaTime, bool isSprinting);
	void handleMouseInput(double xpos, double ypos);
	glm::mat4 getViewMatrix() const;
	glm::vec3 getCameraPos() const;
};

#endif // !CAMERA_H
