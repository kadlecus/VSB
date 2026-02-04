#include "CubemapTexture.h"

CubemapTexture::CubemapTexture(const std::vector<std::string>& faces) {
    if (faces.size() != 6) {
        std::cerr << "Cubemap requires exactly 6 textures!" << std::endl;
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, channels;
    unsigned char* data = nullptr;

    stbi_set_flip_vertically_on_load(false);
   
    GLenum targets[6] = {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    };


    for (int i = 0; i < 6; ++i) {
        data = stbi_load(faces[i].c_str(), &width, &height, &channels, 4);
        if (data) {
            glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Cubemap load failed: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }


    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


CubemapTexture::~CubemapTexture() {
    if (textureID) glDeleteTextures(1, &textureID);
}

void CubemapTexture::bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void CubemapTexture::unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
