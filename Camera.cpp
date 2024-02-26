#include "Camera.h"

const float Camera::cameraSpeed = 2.5f;
const float Camera::sprintSpeed = 5.0f;

Camera::Camera(int window_width, int window_height) :
    cameraPos{ glm::vec3(-4.0f, -3.0f, 1.5f) },
    cameraFront{ glm::vec3(1.0f, 0.0f, 0.0f) },
	cameraUp{ glm::vec3(0.0f, 1.0f, 0.0f) },
    firstMouse{ true },
	yaw{ 0.0f },
	pitch{ 0.0f }
{
	lastX = window_width / 2.0;
	lastY = window_height / 2.0;
}

void Camera::moveForward(float deltaTime, bool isSprinting)
{
    float speed = isSprinting ? sprintSpeed : cameraSpeed;
	cameraPos += speed * deltaTime * cameraFront;
}

void Camera::moveBackward(float deltaTime, bool isSprinting)
{
    float speed = isSprinting ? sprintSpeed : cameraSpeed;
	cameraPos -= speed * deltaTime * cameraFront;
}

void Camera::moveLeft(float deltaTime, bool isSprinting)
{
    float speed = isSprinting ? sprintSpeed : cameraSpeed;
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * deltaTime;
}

void Camera::moveRight(float deltaTime, bool isSprinting)
{
    float speed = isSprinting ? sprintSpeed : cameraSpeed;
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * deltaTime;
}

void Camera::handleMouseInput(double xpos, double ypos)
{
    if (firstMouse) {
        firstMouse = false;
        lastX = xpos;
        lastY = ypos;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::vec3 Camera::getCameraPos() const
{
    return cameraPos;
}
