#include "DrawableObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GL/glew.h>


DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram)
    : model(model)
{
    
    this->shaderProgram = shaderProgram;

    transformation = std::make_shared<Transformation>();   
    inheritTransform = std::make_shared<Transformation>();   
    selfTransform = std::make_shared<Transformation>();
    this->material = new Material(0.3f, 1.0f, 1.0f, 32.0f);
}
void DrawableObject::draw(const glm::mat4& parentMatrix)
{
    glm::mat4 modelMatrix = parentMatrix * transformation->getMatrix() * inheritTransform->getMatrix() * selfTransform->getMatrix();

    if (shaderProgram) {
        shaderProgram->use();
        shaderProgram->setUniform("modelMatrix", modelMatrix);
        shaderProgram->setUniform("objectColor", color);

        if (material)
            shaderProgram->setMaterial(material);

        if (model)
            model->draw();

        shaderProgram->unset();
    }

 
    glm::mat4 childMatrix = parentMatrix * transformation->getMatrix() * inheritTransform->getMatrix();
    for (auto& child : children)
        child->draw(childMatrix);
}



void DrawableObject::addChild(std::shared_ptr<DrawableObject> child)
{
   
    children.push_back(child);
    std::cout << "Child added" << std::endl;
}

void DrawableObject::addShaderProgram(ShaderProgram* shaderProgram)
{
    shaderProgram = shaderProgram;
}

void DrawableObject::setTransformation(std::shared_ptr<Transformation> t)
{
    transformation = t;
}

std::shared_ptr<Transformation> DrawableObject::getTransformation()
{
    return transformation;
}

void DrawableObject::setModel(Model* m)
{
    model = m;
}

void DrawableObject::setColor(const glm::vec3& c)
{
    color = c;
}

glm::vec3 DrawableObject::getColor() 
{
    return color;
}

void DrawableObject::update(float dt)
{
   
    for (auto& comp : transformation->getComponents()) {
        if (auto rot = std::dynamic_pointer_cast<Rotate>(comp)) {
            rot->updateRotation(30.0f * dt);
        }
    }

   
    for (auto& comp : inheritTransform->getComponents()) {
        if (auto rot = std::dynamic_pointer_cast<Rotate>(comp)) {
            rot->updateRotation(30.0f * dt);   
        }
    }

   
    for (auto& comp : selfTransform->getComponents()) {
        if (auto rot = std::dynamic_pointer_cast<Rotate>(comp)) {
            rot->updateRotation(30.0f * dt);   
        }
    }

 
    for (auto& child : children)
        child->update(dt);
}

Model* DrawableObject::getModel()
{
    return this->model;
}

Material* DrawableObject::getMaterial()
{
    return this->material;
}

std::shared_ptr<DrawableObject> DrawableObject::clone() {
    auto copy = std::make_shared<DrawableObject>(model, shaderProgram);
    if (material) copy->setMaterial(new Material(*material));
    copy->setColor(color);

    copy->transformation = transformation->clone();
    copy->inheritTransform = inheritTransform->clone();
    copy->selfTransform = selfTransform->clone();

    return copy;
}


