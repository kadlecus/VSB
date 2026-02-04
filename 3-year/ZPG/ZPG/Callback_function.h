#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "AppContext.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
 
    glViewport(0, 0, width, height);

    AppContext* ctx = reinterpret_cast<AppContext*>(glfwGetWindowUserPointer(window));
    Camera* camera = ctx->camera;
  
    if (camera) {
        camera->setProjection(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
        camera->notify();
    }
}