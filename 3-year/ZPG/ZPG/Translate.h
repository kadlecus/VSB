#pragma once
#include "TransformationComponent.h"
#include <glm/gtc/matrix_transform.hpp>

class Translate : public TransformationComponent {
private:
    glm::vec3 translation;
public:
    Translate(float x, float y, float z) : translation(x, y, z) {}
    Translate(const glm::vec3& t) : translation(t) {}

    void apply(glm::mat4& modelMatrix) override {
        modelMatrix = glm::translate(modelMatrix, translation);
    }

    void setTranslation(float x, float y, float z) {
        translation = glm::vec3(x, y, z);
    }

    glm::vec3 getTranslation()
    {
        return this->translation;
    }

    void setTranslation(const glm::vec3& t) {
        translation = t;
    }
};
