#pragma once

#include "TransformationComponent.h"
#include <glm/glm.hpp>

class CustomTransformation : public TransformationComponent {
public:
    void apply(glm::mat4& modelMatrix) override {
        glm::mat4 custom(1.0f);
        custom[3][3] = 20.0f;   
        modelMatrix = modelMatrix * custom;
    }
};

