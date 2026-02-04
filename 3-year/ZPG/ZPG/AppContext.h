#pragma once
#include "Camera.h"
#include "Scene.h"
#include <vector>
#include "LightManager.h"
#include "ArcadeScene.h"
struct AppContext {
    Camera* camera;
    std::vector<Scene*> scenes;
    std::vector<LightManager*> lightManagers;
    int scene_index;
    std::vector<glm::vec3> clickedPoints;

  
};