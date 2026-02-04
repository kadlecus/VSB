#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>
#include "stb_image.h"

class CubemapTexture {
public:
    CubemapTexture(const std::vector<std::string>& faces);
    ~CubemapTexture();

    void bind(GLenum textureUnit = GL_TEXTURE0) const;
    void unbind() const;

private:
    GLuint textureID = 0;
};
