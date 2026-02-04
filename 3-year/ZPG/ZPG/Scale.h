#pragma once
#include "TransformationComponent.h"
#include <glm/gtc/matrix_transform.hpp>

class Scale : public TransformationComponent {
private:
    glm::vec3 scale;
public:
    Scale(float x, float y, float z) : scale(x, y, z) {}

    void apply(glm::mat4& modelMatrix) override {
        modelMatrix = modelMatrix * glm::scale(glm::mat4(1.0f), scale);
    }

    void setScale(float x, float y, float z) {
        scale = glm::vec3(x, y, z);
    }
};
