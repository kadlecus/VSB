#pragma once
#include "TransformationComponent.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Rotate : public TransformationComponent {
private:
    glm::quat rotation;
    glm::vec3 axis;
    float angularSpeed; 
    float currentAngle; 

public:
    
    Rotate(float angleDeg, const glm::vec3& axis, float speedDegPerSec = 0.0f)
        : axis(glm::normalize(axis)), angularSpeed(speedDegPerSec), currentAngle(angleDeg)
    {
        rotation = glm::angleAxis(glm::radians(angleDeg), this->axis);
    }

    void apply(glm::mat4& modelMatrix) override {
        modelMatrix = modelMatrix * glm::toMat4(rotation);
    }

 
    void updateRotation(float dt) {
        currentAngle += angularSpeed * dt; 
        rotation = glm::angleAxis(glm::radians(currentAngle), axis);
    }

    void setRotation(float angleDeg) {
        currentAngle = angleDeg;
        rotation = glm::angleAxis(glm::radians(angleDeg), axis);
    }

    void setSpeed(float speedDegPerSec) { angularSpeed = speedDegPerSec; }
    float getSpeed() const { return angularSpeed; }
};
