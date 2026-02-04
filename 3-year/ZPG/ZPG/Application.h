#pragma once

#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Model.h"
#include "Scene.h"
#include "Camera.h"
#include "LightManager.h"
#include "AppContext.h"
#include "Light.h"
#include "Controller.h"
#include <glm/glm.hpp>

class Application {
public:
    Application();
    ~Application();

    void initialization();
    void createShaders();
    void createModels();
    void run();

  
    void setScene(Scene* scena) {
        if (scene_switch < scenes.size()) {
            scenes[scene_switch] = scena;
        }
        else {
            scenes.push_back(scena);
        }
       
    }

    void setSceneIndex(int index) {
        if (index >= 0 && index < scenes.size()) {
            scene_switch = index;
        }
    }

    void update(float dt) {
        if (!scenes.empty()) {
            scenes[scene_switch]->update(dt, camera);
        }
    }

    void draw()
    {
        if (!scenes.empty()) {
            scenes[scene_switch]->draw();
        }
    }

private:
    GLFWwindow* window;
    Controller* controller;
    Camera* camera;
    AppContext* appcontext;
    std::vector<Scene*> scenes; 
    SpotLight* spot = nullptr;
    int scene_switch = 0;
    bool q_pressed_last_frame = false;
    bool w_pressed_last_frame = false;
};
