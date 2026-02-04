#pragma once

#include <glm/glm.hpp>

class TransformationComponent {
public:
    virtual void apply(glm::mat4& modelMatrix) = 0;
    virtual void update(float dt) {}
    virtual ~TransformationComponent() {}
}; 
