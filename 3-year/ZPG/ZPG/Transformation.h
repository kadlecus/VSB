#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/component_wise.hpp>

#include "TransformationComponent.h"
#include <vector>
#include <memory>
#include "Translate.h"

#include "Rotate.h"
#include "Translate.h"
#include "Scale.h"
#include"PathMove.h"

class Transformation {
public:
    void addComponent(std::shared_ptr<TransformationComponent> comp) {
        components.push_back(comp);
    }

    void clear() {
        components.clear();
    }


    glm::mat4 getMatrix() const {
        glm::mat4 M(1.0f);
        for (auto& comp : components) {
            comp->apply(M);
        }
        return M;
    }

    std::vector<std::shared_ptr<TransformationComponent>> getComponents()
    {
        return this->components;
    }

    glm::vec3 getTranslation(){
        for (auto& comp : components) {
            if (auto t = std::dynamic_pointer_cast<Translate>(comp))
                return t->getTranslation();
        }
        return glm::vec3(0.0f);
    }

    std::shared_ptr<Transformation> clone() const {
        auto copy = std::make_shared<Transformation>();
        for (auto& comp : components) {
            if (auto t = std::dynamic_pointer_cast<Translate>(comp))
                copy->addComponent(std::make_shared<Translate>(*t));
            else if (auto r = std::dynamic_pointer_cast<Rotate>(comp))
                copy->addComponent(std::make_shared<Rotate>(*r));
            else if (auto s = std::dynamic_pointer_cast<Scale>(comp))
                copy->addComponent(std::make_shared<Scale>(*s));
            else if (auto m = std::dynamic_pointer_cast<PathMove>(comp))
                copy->addComponent(std::make_shared<PathMove>(*m));
            
        }
        return copy;
    }

private:
    std::vector<std::shared_ptr<TransformationComponent>> components;
};
