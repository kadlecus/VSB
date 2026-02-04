#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "ISubject.h"

class Camera : public ISubject
{
public:
    Camera();

    void setProjection(float fov, float aspect, float near, float far);
    void rotateCamera(float deltaYaw, float deltaPitch);
    void setPosition(const glm::vec3& pos);
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    glm::mat4 getCamera();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();

    glm::vec3 getPosition() const { return this->eye; }
    glm::vec3 getFront() const {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        return glm::normalize(front);
    }

    void updateDirection();

    void setResolution(int width, int height)
    {
        this->resolution = { width, height };
        notify();
    }

    glm::vec2 getResolution()
    {
        return this->resolution;
    }

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    float pitch; // rotation around X
    float yaw; //rotation around Y 
    float speed = 2.5f;

    glm::vec3 eye; // camera position in the world
    glm::vec3 target; // point the camera is looking at
    glm::vec3 up;  
 
    glm::vec2 resolution;
  
};
