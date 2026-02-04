#include "DrawableObject.h"
#include "CubemapTexture.h"
#include "AppContext.h"

class SkyboxObject : public DrawableObject {
public:
    SkyboxObject(Model* model, CubemapTexture* cubemap, ShaderProgram* shader, Camera* cam)
        : model(model), cubemap(cubemap), shader(shader), camera(cam) {}

    void draw(const glm::mat4& parentMatrix) override {
        glDepthFunc(GL_LEQUAL);
        shader->use();

        glm::mat4 viewNoPos = glm::mat4(glm::mat3(camera->getViewMatrix()));
        shader->setUniform("view", viewNoPos);
        shader->setUniform("projection", camera->getProjectionMatrix());

        cubemap->bind(GL_TEXTURE0);

        if (model)
            model->draw();

        glDepthFunc(GL_LESS);
    }

private:
    Model* model;
    CubemapTexture* cubemap;
    ShaderProgram* shader;
    Camera* camera;
};
