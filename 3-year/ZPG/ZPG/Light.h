#pragma once

#include <glm/glm.hpp>
#include "ISubject.h"
#include "DrawableObject.h"
#include "Model.h"

class Light : public ISubject, public DrawableObject {
public:

    enum LightType {

        POINT = 0,
        DIRECTIONAL = 1,
        SPOT = 2,
    };

    Light(Model* model = nullptr, ShaderProgram* shaderprogram = nullptr) : DrawableObject(model, shaderprogram) {}

    virtual ~Light() {}

    virtual void update(float dt) = 0;

    virtual void setPosition(const glm::vec3& pos) = 0;
    virtual void setColor(const glm::vec3& col) = 0;
    virtual void setIntensity(float inten) = 0;
    virtual glm::vec3 getAttenuation() const = 0;

    virtual void setAttenuation(const glm::vec3& att) = 0;
    virtual glm::vec3 getPosition() const = 0;
    virtual glm::vec3 getColor() const = 0;
    virtual LightType getType() const = 0;
    virtual float getIntensity() const = 0;
};
