#pragma once
#include "DrawableObject.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "LightManager.h"
#include "SpotLight.h"
#include "Texture.h"


using namespace std;

class Scene {
private:
    std::vector<std::shared_ptr<DrawableObject>> objects; 
    std::vector<ShaderProgram*> shaderprograms;
    LightManager* lightManager = nullptr; 
    
public:
    Scene() {}
    ~Scene() {
        for (auto shader : shaderprograms)
            delete shader;
   
    }

    void setLightManager(LightManager* lm) {
        lightManager = lm;
    }

    LightManager* getLightManager()
    {
        return this->lightManager;
    }

    void addLight(Light* light) {
       // std::cout << "[LIGHT] " << lightManager << std::endl;
        if (!lightManager) {
            std::cerr << "[LIGHT] LightManager not set!\n";
            return;
        }

       lightManager->addLight(light);

     
        for (auto shader : shaderprograms) {
            light->attach(shader);
            shader->update(lightManager);
        }
    }

    int getObjectsCount() { return objects.size(); }

   
    
    void addShaderProgram(ShaderProgram* shader) {
        shaderprograms.push_back(shader);
        if (lightManager) {
            lightManager->attach(shader);
            shader->update(lightManager);
        }
    }

    void addObject(std::shared_ptr<DrawableObject> obj)
    {
        obj->setID(static_cast<int>(this->objects.size()) + 1);
        this->objects.push_back(obj);
        printf("Object %d added\n", obj->getId());
    }

   
    ShaderProgram* getShaderProgram(int index)
    {
        if (index < 0 || index >= shaderprograms.size()) {
            std::cerr << "Shader index out of range!" << std::endl;
            return nullptr;
        }
        return shaderprograms[index];
    }

    void addObjects(const float* vertices, int size, Texture* tex, int stride, const glm::vec3& position,
        const glm::vec3& scale, const glm::vec3& color,
        Material* material, int shaderIndex = 0)
    {
        if (shaderIndex < 0 || shaderIndex >= shaderprograms.size()) {
            std::cerr << "Shader index out of range!" << std::endl;
            return;
        }

        Model* model = new Model(vertices, size, tex, stride);

        std::shared_ptr<Transformation> transform = std::make_shared<Transformation>();
        transform->addComponent(std::make_shared<Translate>(position.x, position.y, position.z));
        transform->addComponent(std::make_shared<Scale>(scale.x, scale.y, scale.z));

        auto obj = std::make_shared<DrawableObject>(model, shaderprograms[shaderIndex]);
        obj->setTransformation(transform);
        obj->setMaterial(material);
        obj->setColor(color);
        obj->setID(static_cast<int>(objects.size()) + 1);
        printf("%d\n\n", obj->getId());
        objects.push_back(obj);
    }

      virtual void update(float dt, Camera* camera) {
        for (auto& obj : objects)
            obj->update(dt);

        for (auto& obj : objects)
        {
            auto tr = obj->getTransformation();
            if (!tr) continue;

            for (auto& comp : tr->getComponents())
                comp->update(dt);   
        }


        for (auto& light : lightManager->getLights())
        {
            light->update(dt);
          
            if (auto* spot = dynamic_cast<SpotLight*>(light)) {
                spot->setPositionAndDirection(camera->getPosition(), camera->getFront());
            }
        }

       

    }

    void draw() {
        glm::mat4 identity(1.0f);

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glStencilMask(0xFF);

        for (auto& obj : objects)
        {
            glStencilFunc(GL_ALWAYS, obj->getId(), 0xFF);
            obj->draw(identity); 
        }

        glStencilMask(0x00);
        glDisable(GL_STENCIL_TEST);

        for (auto& lig : lightManager->getLights())
        {
            lig->draw(identity);
        }
        lightManager->notify();
    }


    void removeObjectById(int objectId)
    {
        for (auto it = objects.begin(); it != objects.end(); ) {
            if ((*it)->getId() == objectId) {
                it = objects.erase(it);
            }
            else {
                ++it; 
            }
        }
    }

    
    std::shared_ptr<DrawableObject> addObjectAt(const float* vertices, size_t size, int stride, const glm::vec3& pos, ShaderProgram* shader)
    {
        Model* model = new Model(vertices, size, nullptr, stride);
        shader->use();
        auto obj = std::make_shared<DrawableObject>(model, shader);
        obj->setTransformation(std::make_shared<Transformation>());
        obj->getTransformation()->addComponent(std::make_shared<Translate>(pos.x, pos.y, pos.z));
        addObject(obj);

        return obj;
        
    }

    std::shared_ptr<DrawableObject> addObjectAt(std::shared_ptr<DrawableObject> obj, const glm::vec3& pos)
    {
       
        obj->setTransformation(std::make_shared<Transformation>());
        obj->getTransformation()->addComponent(std::make_shared<Translate>(pos.x, pos.y, pos.z));
        addObject(obj);

        return obj;

    }

    std::shared_ptr<DrawableObject> getObjectById(int id) {
        for (auto& obj : objects) {
            if (obj->getId() == id)  
                return obj;
        }
        return nullptr; 
    }

    void clear() {
        objects.clear(); 
    }

    const std::vector<std::shared_ptr<DrawableObject>>& getObjects() const {
        return objects;
    }

};
