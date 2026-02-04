#pragma once
#include "Transformation.h"
#include "Model.h"
#include "ShaderProgram.h"

#include "Rotate.h"
#include "Scale.h"
#include "Translate.h"
#include "CustomTransform.h"
#include "Material.h"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

class DrawableObject {
public:
    DrawableObject(Model* model = nullptr, ShaderProgram* shaderProgram = nullptr);
   // DrawableObject(Model* model = nullptr, ShaderProgram* shaderProgram = nullptr, Material* material = nullptr);

    int getId() { return this->objectId; }
    void setID(int id) { this->objectId = id; }

    virtual void draw(const glm::mat4& parentMatrix);
    void addChild(std::shared_ptr<DrawableObject> child);
    void addShaderProgram(ShaderProgram* shaderProgram);
    void setTransformation(std::shared_ptr<Transformation> transformation);
    void setModel(Model* model);
    void setMaterial(Material* mat) {
        this->material = mat;
    }
    void update(float dt);

    std::shared_ptr<DrawableObject> clone();

    std::shared_ptr<Transformation> getTransformation();

    void setColor(const glm::vec3& color);
    glm::vec3 getColor();
    Model* getModel();
    Material* getMaterial();
    ShaderProgram* getShaderProgram() { return this->shaderProgram; }
    std::shared_ptr<Transformation> getInheritTransformation() { return this->inheritTransform; }
    std::shared_ptr<Transformation> getSelfTransformation() { return this->selfTransform; }

    
protected:
    std::shared_ptr<Transformation> inheritTransform;  
    std::shared_ptr<Transformation> selfTransform;       
    std::shared_ptr<Transformation> transformation;

private:
    int objectId = 0;
    Model* model;
    Material* material = nullptr;
    ShaderProgram* shaderProgram;
    std::vector<std::shared_ptr<DrawableObject>> children;
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};
