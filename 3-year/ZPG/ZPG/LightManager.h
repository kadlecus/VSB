#pragma once
#include "Light.h"
#include "ShaderProgram.h"
#include <vector>
#include <memory>

class LightManager : public ISubject{
private:
    std::vector<Light*> lights;
public:
    void addLight(Light* light) {
        lights.push_back(light);
        notify();
    }

    void setLightPosition(int index, const glm::vec3& pos) {
        if (index >= 0 && index < lights.size()) {
            lights[index]->setPosition(pos);
            notify();
        }
    }

    void setLightColor(int index, const glm::vec3& col) {
        if (index >= 0 && index < lights.size()) {
            lights[index]->setColor(col);
            notify();
        }
    }

    void setLightIntensity(int index, float intensity) {
        if (index >= 0 && index < lights.size()) {
            lights[index]->setIntensity(intensity);
            notify();
        }
    }

    const std::vector<Light*>& getLights() const { return lights; }
};
