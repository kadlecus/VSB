#include "Controller.h"
#include "AppContext.h"
#include "Scene.h"
#include "Models/tree.h"
#include <iostream>
#include "BezierSpline.h"
#include "BezierSplineMove.h"

bool Controller::rightButtonPressed = false;
double Controller::lastX = 400, Controller::lastY = 300;
std::shared_ptr<DrawableObject> Controller::copiedObject = nullptr;
std::shared_ptr<DrawableObject> Controller::selectedObject = nullptr;
bool Controller::dragging = false;
bool fPreviouslyPressed = false;
bool cPreviouslyPressed = false;

void Controller::processInput(GLFWwindow* window, Camera* camera, double deltaTime, int& scene_index) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->moveRight(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) scene_index = 0;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) scene_index = 1;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) scene_index = 2;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) scene_index = 3;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) scene_index = 4;
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) scene_index = 5;

    AppContext* ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));
    LightManager* lm = ctx->lightManagers[scene_index];

    SpotLight* spotlight = nullptr;
    
    for (Light* l : lm->getLights()) {
        if (l->getType() == Light::LightType::SPOT) {
            spotlight = dynamic_cast<SpotLight*>(l);
            break;
        }
    }

    bool fPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
    if (fPressed && !fPreviouslyPressed) {
        if (spotlight) spotlight->toggle();
    }
    fPreviouslyPressed = fPressed;

    bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
    if (cPressed && !cPreviouslyPressed) {   
        if (selectedObject) {
            copiedObject = selectedObject->clone();
            printf("Object %d copied\n", copiedObject->getId());
        }
        else {
            printf("No object selected to copy!\n");
        }
    }
    cPreviouslyPressed = cPressed;

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (selectedObject && ctx->clickedPoints.size() >= 4) {

            BezierSpline spline(ctx->clickedPoints);
            auto move = std::make_shared<BezierSplineMove>(spline, 0.15f);
     
            selectedObject->getTransformation()->addComponent(move);

            std::cout << "Spline added to object. Points: "
                << ctx->clickedPoints.size() << "\n";

            ctx->clickedPoints.clear();
        }
    }
}

void Controller::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    AppContext* ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));
    if (!ctx) return;

    Camera* camera = ctx->camera;
    if (!camera) return;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (rightButtonPressed) {
        camera->rotateCamera(xoffset * 0.1f, yoffset * 0.1f);
    }

    
    if (dragging && selectedObject) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        GLfloat depth;
        glReadPixels(static_cast<GLint>(xpos), height - static_cast<GLint>(ypos), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        glm::vec3 screenPos(xpos, height - ypos, depth);
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();
        glm::vec4 viewport(0, 0, width, height);

        glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);

        auto transform = selectedObject->getTransformation();
        if (transform) {
            for (auto& comp : transform->getComponents()) {
                auto translate = std::dynamic_pointer_cast<Translate>(comp);
                if (translate) {
                    translate->setTranslation(worldPos);
                    break;
                }
            }
        }
    }
}

void Controller::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    AppContext* ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));
    if (!ctx) return;

    Camera* camera = ctx->camera;
    Scene* currentScene = ctx->scenes[ctx->scene_index];
    if (!currentScene) return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    GLint x = static_cast<GLint>(xpos);
    GLint y = static_cast<GLint>(ypos);
    GLint newY = height - y;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        GLbyte color[4];
        GLuint id;
        GLfloat depth;

        glReadPixels(x, newY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
        glReadPixels(x, newY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &id);
        glReadPixels(x, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
            x, y, color[0], color[1], color[2], color[3], depth, id);

        selectedObject = currentScene->getObjectById(id);

        dragging = (selectedObject != nullptr);

        ArcadeScene* arcadeScene = dynamic_cast<ArcadeScene*>(currentScene);
        if (arcadeScene && id != 0) arcadeScene->onObjectClicked(id);

        bool shiftPressed = (mods & GLFW_MOD_SHIFT) != 0;
        bool ctrlPressed = (mods & GLFW_MOD_CONTROL) != 0;

        glm::vec3 screenPos(xpos, height - ypos, depth);
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();
        glm::vec4 viewport(0, 0, width, height);
        glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);

        if (!shiftPressed && !ctrlPressed) {
            ctx->clickedPoints.push_back(worldPos);
            std::cout << "POINT ADDED: "
                << worldPos.x << " " << worldPos.y << " " << worldPos.z
                << "\n";
        }

        if (shiftPressed && selectedObject) {
            currentScene->removeObjectById(selectedObject->getId());
            selectedObject = nullptr;
            dragging = false;
        }
        else if (ctrlPressed) {
            if (copiedObject) {
                auto obj = copiedObject->clone();
                currentScene->addObjectAt(obj, worldPos);
                printf("Object added at x %f y %f z %f with id %d\n", worldPos.x, worldPos.y, worldPos.z, obj->getId());
            }
            else {
                printf("No object copied! Press C on a selected object first.\n");
            }
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightButtonPressed = (action == GLFW_PRESS);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        dragging = false;
    }
}

