#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Texture.h"

class Model {
public:
    Model(const float* vertices, int size);
    Model(const float* vertices, size_t size, Texture* texture, int stride);
    Model(const char* name, Texture* texture);
    ~Model();
    void draw();

private:
    GLuint VAO, VBO, EBO;
    int vertexCount;
    Texture* texture = nullptr;
};
