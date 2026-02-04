#pragma once
#include "Light.h"
#include "Transformation.h"
#include "Translate.h"
#include "Scale.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "PathMove.h"

class PointLight : public Light {
public:

 
    PointLight(const glm::vec3& position,
        const glm::vec3& color,
        float intensity = 1.0f,
        glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f),
        Model* model = nullptr,
        ShaderProgram* shaderprogram = nullptr)
        : Light(model, shaderprogram),
        position(position),
        color(color),
        intensity(intensity),
        attenuation(attenuation),
        origin(position)
    {
        setTransformation(std::make_shared<Transformation>());
        getTransformation()->addComponent(std::make_shared<Translate>(position.x, position.y, position.z));
        getTransformation()->addComponent(std::make_shared<Scale>(0.2f, 0.2f, 0.2f));
    }

  
    PointLight(const glm::vec3& position,
        const glm::vec3& color,
        float intensity = 1.0f,
        glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f),
        bool usePathMove = false,
        Model* model = nullptr,
        ShaderProgram* shaderprogram = nullptr)
        : Light(model, shaderprogram),
        position(position),
        color(color),
        intensity(intensity),
        attenuation(attenuation),
        origin(position)
    {
        setTransformation(std::make_shared<Transformation>());
        getTransformation()->addComponent(std::make_shared<Translate>(position.x, position.y, position.z));
        getTransformation()->addComponent(std::make_shared<Scale>(0.2f, 0.2f, 0.2f));

        if (usePathMove) {
           
            std::vector<glm::vec3> path;
            path.push_back(position);
            for (int i = 0; i < 2; i++)
            {
                float nx = position.x + ((rand() % 200) - 100) / 15.0f;
                float ny = position.y + ((rand() % 200) - 100) / 15.0f;
                float nz = position.z + ((rand() % 200) - 100) / 15.0f;
                path.push_back(glm::vec3(nx, ny, nz));
            }
            path.push_back(position); 

            getTransformation()->addComponent(std::make_shared<PathMove>(path, 0.4f));
        }
    }


    void update(float dt) override {
        DrawableObject::update(dt);

       
        auto transform = getTransformation();
        if (transform && !transform->getComponents().empty()) {
            for (auto& comp : transform->getComponents()) {
                if (auto pathMove = std::dynamic_pointer_cast<PathMove>(comp)) {
                    pathMove->update(dt);

                    glm::mat4 model = glm::mat4(1.0f);
                    pathMove->apply(model);

                   
                    auto translateComp = std::dynamic_pointer_cast<Translate>(transform->getComponents()[0]);
                    if (translateComp) {
                        translateComp->setTranslation(model[3][0], model[3][1], model[3][2]);
                        position = glm::vec3(model[3]); 
                    }
                }
            }
        }

        notify();
    }

    void setPosition(const glm::vec3& pos) override {
        position = pos;

        auto transform = getTransformation();
        if (!transform->getComponents().empty()) {
            auto translateComp = std::dynamic_pointer_cast<Translate>(transform->getComponents()[0]);
            if (translateComp) translateComp->setTranslation(pos.x, pos.y, pos.z);
        }

        notify();
    }

    void setAttenuation(const glm::vec3& att) override {
        attenuation = att;
        notify();
    }

    glm::vec3 getAttenuation() const override {
        return attenuation;
    }

    void setColor(const glm::vec3& col) override { color = col; notify(); }
    void setIntensity(float inten) override { intensity = inten; notify(); }

    glm::vec3 getPosition() const override { return position; }
    glm::vec3 getColor() const override { return color; }
    float getIntensity() const override { return intensity; }
    LightType getType() const override { return LightType::POINT; }

private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 origin;
    glm::vec3 attenuation;

   
    float time = 0.0f;
};
