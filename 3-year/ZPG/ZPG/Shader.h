#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
    Shader();
    Shader(const char* shaderString, GLenum type);

    //pridano
    void createShader(GLenum shaderType, const char* shaderCode);

    void createShaderFromFile(GLenum shaderType, const char* shaderFile);

    void attachShader(GLuint idShaderProgram);

    GLuint getId() const
    {
        return id;
    }

private:
    GLuint id;
};
