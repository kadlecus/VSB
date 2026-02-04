#pragma once
#include "Light.h"
#include "Transformation.h"
#include "Translate.h"
#include "Scale.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

class SpotLight : public Light {
public:


    SpotLight(const glm::vec3& position,
        const glm::vec3& direction,
        float cutOffAngle,
        const glm::vec3& color,
        float intensity = 1.0f,
        glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f),
        Model* model = nullptr,
        ShaderProgram* shaderprogram = nullptr)
        : Light(model, shaderprogram),
        position(position),
        direction(glm::normalize(direction)),
        cutOff(cutOffAngle),
        color(color),
        intensity(intensity),
        attenuation(attenuation),
        enabled(true)
    {
        setTransformation(std::make_shared<Transformation>());
        getTransformation()->addComponent(std::make_shared<Translate>(position.x, position.y, position.z));
        getTransformation()->addComponent(std::make_shared<Scale>(0.3f, 0.3f, 0.3f));
    }

    void toggle() { enabled = !enabled; notify(); }

    void setPositionAndDirection(const glm::vec3& pos, const glm::vec3& dir) {
        position = pos;
        direction = glm::normalize(dir);
        updateTransform();
        notify();
    }

    void update(float dt) override {
        notify();
    }

   
    glm::vec3 getPosition() const override { return position; }
    glm::vec3 getDirection() const { return direction; }
    glm::vec3 getColor() const override { return enabled ? color : glm::vec3(0.0f); }
    float getIntensity() const override { return enabled ? intensity : 0.0f; }
    LightType getType() const override { return LightType::SPOT; }
    glm::vec3 getAttenuation() const override { return attenuation; }
    float getCutOff() const { return cutOff; }
    bool getEnabled() const { return enabled; }

 
    void setColor(const glm::vec3& col) override { color = col; notify(); }
    void setIntensity(float inten) override { intensity = inten; notify(); }

    void setAttenuation(const glm::vec3& att) override {
        attenuation = att;
        notify();
    }

    void setPosition(const glm::vec3& pos) override {
        position = pos;
        updateTransform();
        notify();
    }

    void setDirection(const glm::vec3& dir) {
        direction = glm::normalize(dir);
        notify();
    }

    void setCutOff(float angle) {
        cutOff = angle;
        notify();
    }

private:
    void updateTransform() {
        auto tr = getTransformation();
        if (!tr->getComponents().empty()) {
            auto translate = std::dynamic_pointer_cast<Translate>(tr->getComponents()[0]);
            if (translate) translate->setTranslation(position.x, position.y, position.z);
        }
    }

private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    glm::vec3 attenuation;

    float intensity;
    float cutOff;
    bool enabled;
};
