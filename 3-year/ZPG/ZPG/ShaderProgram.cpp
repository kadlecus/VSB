#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "LightManager.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"


#include <stdexcept>
#include <iostream>
#include <string>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


ShaderProgram::ShaderProgram(Shader* vertexSrc, Shader* fragmentSrc) {

    programShaderId = glCreateProgram();

    this->fragmentSrc = fragmentSrc;
    this->vertexSrc = vertexSrc;

    this->vertexSrc->attachShader(this->programShaderId);
    this->fragmentSrc->attachShader(this->programShaderId);
    glLinkProgram(programShaderId);

}

void ShaderProgram::use() {
    glUseProgram(programShaderId);
}

bool ShaderProgram::checkError()
{
    GLint success;
    glGetProgramiv(programShaderId, GL_LINK_STATUS, &success);


    if (!success) {
        GLint logLength = 0;
        glGetProgramiv(programShaderId, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(logLength, '\0');
        glGetProgramInfoLog(programShaderId, logLength, nullptr, &log[0]);

        std::cerr << "Shader program link error:\n" << log << std::endl;

       
        GLint numUniforms = 0;
        glGetProgramiv(programShaderId, GL_ACTIVE_UNIFORMS, &numUniforms);
        std::cerr << "Active uniforms (" << numUniforms << "):\n";
        for (GLint i = 0; i < numUniforms; ++i) {
            char name[256];
            GLsizei length;
            GLint size;
            GLenum type;
            glGetActiveUniform(programShaderId, i, 256, &length, &size, &type, name);
            std::cerr << "  " << name << " (size=" << size << ", type=" << type << ")\n";
        }

        return true;
    }
    else {
        std::cout << "Shader program linked successfully!\n";

        GLint numUniforms = 0;
        glGetProgramiv(programShaderId, GL_ACTIVE_UNIFORMS, &numUniforms);
        std::cout << "Active uniforms (" << numUniforms << "):\n";
        for (GLint i = 0; i < numUniforms; ++i) {
            char name[256];
            GLsizei length;
            GLint size;
            GLenum type;
            glGetActiveUniform(programShaderId, i, 256, &length, &size, &type, name);
            std::cout << "  " << name << " (size=" << size << ", type=" << type << ")\n";
        }

        return false;
    }
}

void ShaderProgram::setUniform(const char* name, float value)
{
    GLint location = glGetUniformLocation(programShaderId, name);

    if (location == -1) {
       // std::cerr << "Warning: uniform '" << name << "' not found or not used in shader.\n";
        return;
    }

    glUniform1f(location, value);
}

void ShaderProgram::setUniform(const char* name, const glm::vec3& value)
{
 
    GLint location = glGetUniformLocation(programShaderId, name);
    if (programShaderId == 0) {
        std::cerr << "ERROR: programShaderId == 0 !!\n";
    }

    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' not found or not used in shader.\n";
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char* name, const glm::mat4& value) {
    GLint location = glGetUniformLocation(programShaderId, name);
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' not found.\n";
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setMaterial(Material* mat) {
    if (!mat)
        return;

    
    setUniform("material.ra", mat->ra);
    setUniform("material.rd", mat->rd);
    setUniform("material.rs", mat->rs);
    setUniform("material.shininess", mat->shininess);
    
}

void ShaderProgram::update(ISubject* subject)
{
    this->use();

  
    if (auto camera = dynamic_cast<Camera*>(subject)) {
        GLint viewLoc = glGetUniformLocation(programShaderId, "viewMatrix");
        GLint projLoc = glGetUniformLocation(programShaderId, "projectionMatrix");

   
        if (viewLoc != -1)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
        if (projLoc != -1)
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
    }
    else if (auto lm = dynamic_cast<LightManager*>(subject)) {
        const auto& lights = lm->getLights();
        int numLights = static_cast<int>(lights.size());

        GLint numLightsLoc = glGetUniformLocation(programShaderId, "numLights");
        if (numLightsLoc != -1)
            glUniform1i(numLightsLoc, numLights);

       
        for (int i = 0; i < numLights; ++i) {
            auto* light = lights[i];
            std::string base = "lights[" + std::to_string(i) + "]";

           
            GLint typeLoc = glGetUniformLocation(programShaderId, (base + ".type").c_str());
            if (typeLoc != -1)
                glUniform1i(typeLoc, static_cast<int>(light->getType()));

            // POINT LIGHT
            if (auto* point = dynamic_cast<PointLight*>(light)) {
                GLint posLoc = glGetUniformLocation(programShaderId, (base + ".position").c_str());
                GLint colorLoc = glGetUniformLocation(programShaderId, (base + ".color").c_str());
                GLint intensityLoc = glGetUniformLocation(programShaderId, (base + ".intensity").c_str());
                GLint attenuationLoc = glGetUniformLocation(programShaderId, (base + ".attenuation").c_str());

                if (posLoc != -1)
                    glUniform3fv(posLoc, 1, glm::value_ptr(point->getPosition()));
                if (colorLoc != -1)
                    glUniform3fv(colorLoc, 1, glm::value_ptr(point->getColor()));
                if (intensityLoc != -1)
                    glUniform1f(intensityLoc, point->getIntensity());
                if (attenuationLoc != -1)
                    glUniform3fv(attenuationLoc, 1, glm::value_ptr(point->getAttenuation()));
            }
            // DIRECTIONAL LIGHT
            else if (auto* dir = dynamic_cast<DirectionalLight*>(light)) {
                GLint dirLoc = glGetUniformLocation(programShaderId, (base + ".direction").c_str());
                GLint colorLoc = glGetUniformLocation(programShaderId, (base + ".color").c_str());
                GLint intensityLoc = glGetUniformLocation(programShaderId, (base + ".intensity").c_str());

                if (dirLoc != -1)
                    glUniform3fv(dirLoc, 1, glm::value_ptr(dir->getDirection()));
                if (colorLoc != -1)
                    glUniform3fv(colorLoc, 1, glm::value_ptr(dir->getColor()));
                if (intensityLoc != -1)
                    glUniform1f(intensityLoc, dir->getIntensity());
            }
            //SPOT LIGHT
            else if (auto* spot = dynamic_cast<SpotLight*>(light)) {
               
                GLint posLoc = glGetUniformLocation(programShaderId, (base + ".position").c_str());
                GLint dirLoc = glGetUniformLocation(programShaderId, (base + ".direction").c_str());
                GLint colorLoc = glGetUniformLocation(programShaderId, (base + ".color").c_str());
                GLint intensityLoc = glGetUniformLocation(programShaderId, (base + ".intensity").c_str());
                GLint cutOffLoc = glGetUniformLocation(programShaderId, (base + ".cutOff").c_str());
                GLint enabledLoc = glGetUniformLocation(programShaderId, (base + ".enabled").c_str());
                GLint attenuationLoc = glGetUniformLocation(programShaderId, (base + ".attenuation").c_str());

                if (posLoc != -1)
                    glUniform3fv(posLoc, 1, glm::value_ptr(spot->getPosition()));
                if (dirLoc != -1)
                    glUniform3fv(dirLoc, 1, glm::value_ptr(spot->getDirection()));
                if (colorLoc != -1)
                    glUniform3fv(colorLoc, 1, glm::value_ptr(spot->getColor()));
                if (intensityLoc != -1)
                    glUniform1f(intensityLoc, spot->getIntensity());
                if (cutOffLoc != -1)
                    glUniform1f(cutOffLoc, glm::cos(glm::radians(spot->getCutOff())));
                if (enabledLoc != -1)
                    glUniform1i(enabledLoc, spot->getEnabled() ? 1 : 0);
                if (attenuationLoc != -1)
                    glUniform3fv(attenuationLoc, 1, glm::value_ptr(spot->getAttenuation()));
            }
        }
    }
}







