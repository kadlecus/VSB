#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Camera.h"
#include "SpotLight.h"
#include "Scene.h"
#include "DrawableObject.h"

class Controller {
public:
    static void processInput(GLFWwindow* window, Camera* camera,  double deltaTime, int& scene_index);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

private:
    static bool rightButtonPressed;
    static double lastX, lastY;

 
    static std::shared_ptr<DrawableObject> selectedObject;
    static std::shared_ptr<DrawableObject> copiedObject;
    static bool dragging;
};
