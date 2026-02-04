#include "Camera.h"
#include "ShaderProgram.h"
#include <iostream>


Camera::Camera() : pitch(0.0f), yaw(-90.0f), speed(5.0f), eye(glm::vec3(0.0f, 0.0f, 3.0f)), target(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f))
{

	viewMatrix = glm::lookAt(eye, eye + target, up);
	projectionMatrix = glm::mat4(1.0f);
	notify();
}

void Camera::setProjection(float fov, float aspect, float near, float far)
{
	projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
	notify();
}

void Camera::setPosition(const glm::vec3& pos) {
	eye = pos;
	updateDirection(); 
	notify();
}

void Camera::updateDirection()
{
	float alpha = glm::radians(yaw);
	float fi = glm::radians(pitch);

	glm::vec3 newDir;
	newDir.x = cos(alpha) * cos(fi);
	newDir.y = sin(fi);
	newDir.z = sin(alpha) * cos(fi);

	target = glm::normalize(newDir);
	viewMatrix = glm::lookAt(eye, eye + target, up);

	std::cout << "Camera dir: " << target.x << ", " << target.y << ", " << target.z << std::endl;
	notify();
}

void Camera::rotateCamera(float deltaYaw, float deltaPitch)
{
	this->pitch += deltaPitch;
	this->yaw += deltaYaw;

	if (this->pitch > 89.0f)
	{
		this->pitch = 89.0f;
	}
	if (this->pitch < -89.0f)
	{
		this->pitch = -89.0f;
	}

	this->updateDirection();
	
}

glm::mat4 Camera::getCamera() {
	return glm::lookAt(eye, eye + target, up);
}


glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;;
}


void Camera::moveForward(float deltaTime) {
	float velocity = speed * deltaTime;
	eye += target * velocity;
	std::cout << "W pressed - moving forward\n" << velocity;
	updateDirection();
	notify();
}

void Camera::moveBackward(float deltaTime) {
	float velocity = speed * deltaTime;
	eye -= target * velocity;
	std::cout << "S pressed - moving backward\n";
	updateDirection();
	notify();
}

void Camera::moveLeft(float deltaTime) {
	float velocity = speed * deltaTime;
	glm::vec3 left = glm::normalize(glm::cross(up, target));
	eye += left * velocity;
	std::cout << "A pressed - moving left\n";
	updateDirection();
	notify();
}

void Camera::moveRight(float deltaTime) {
	float velocity = speed * deltaTime;
	glm::vec3 right = glm::normalize(glm::cross(target, up));
	eye += right * velocity;
	std::cout << "D pressed - moving right\n";
	updateDirection();
	notify();
}

