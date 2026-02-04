#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include "Shader.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "IObserver.h"
#include <memory>
#include <vector>
#include "ISubject.h"
#include "Material.h"

class Camera;

class ShaderProgram : public IObserver {
public:
    ShaderProgram(Shader* vertexSrc, Shader* fragmentSrc);
    void unset() { glUseProgram(0); }
    void use();
    bool checkError();
    void setUniform(const char* name, const glm::vec3& value);
    void setUniform(const char* name, const glm::mat4& value);
    void setUniform(const char* name, float value);
    void setMaterial(Material* mat);

    void update(ISubject* subject) override;


private:
    GLuint programShaderId;
    Shader* vertexSrc;
    Shader* fragmentSrc;
};
