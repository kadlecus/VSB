#pragma once
#include "Light.h"
#include <glm/glm.hpp>
#include <memory>

class DirectionalLight : public Light {
public:
    DirectionalLight(const glm::vec3& direction,
        const glm::vec3& color,
        float intensity = 1.0f,
        Model* model = nullptr,
        ShaderProgram* shaderprogram = nullptr)
        : Light(model, shaderprogram),
        direction(glm::normalize(direction)),
        color(color),
        intensity(intensity)
    {
       
    }

    glm::vec3 getAttenuation() const override {
        return glm::vec3(0.0f);
    }

   
    void setAttenuation(const glm::vec3& att) override {}

    void update(float dt) override {
        
        notify();
    }

    void setDirection(const glm::vec3& dir) {
        direction = glm::normalize(dir);
        notify();
    }

    glm::vec3 getDirection() const { return direction; }
    LightType getType() const override { return LightType::DIRECTIONAL; }

    void setPosition(const glm::vec3& pos) override {
    }

    glm::vec3 getPosition() const override {
        return glm::vec3(0.0f);
    }

    void setColor(const glm::vec3& col) override { color = col; notify(); }
    void setIntensity(float inten) override { intensity = inten; notify(); }
    glm::vec3 getColor() const override { return color; }
    float getIntensity() const override { return intensity; }

private:
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};
