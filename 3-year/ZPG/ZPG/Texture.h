#pragma once
#include <Gl/glew.h>
#include <string>
#include <iostream>

class Texture {
public:
    Texture(const std::string& path, GLenum textureUnit = GL_TEXTURE0);
    ~Texture();

    void bind() const;
    void unbind() const;

    GLuint getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    GLuint textureID = 0;
    int width = 0, height = 0, channels = 0;
    GLenum unit;
};

