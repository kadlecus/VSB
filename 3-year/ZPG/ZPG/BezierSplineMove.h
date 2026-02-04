#pragma once
#include "TransformationComponent.h"
#include "BezierSpline.h"
#include <glm/gtc/matrix_transform.hpp>


class BezierSplineMove : public TransformationComponent {
private:
    BezierSpline spline;
    float t = 0.0f;
    float speed;

public:
    BezierSplineMove(const BezierSpline& spline, float speed = 0.2f) : spline(spline), speed(speed) {}



    void update(float dt) override {
        if (!spline.isValid()) return;
        t += speed * dt;
        if (t > 1.0f) t -= 1.0f;
    }

    void apply(glm::mat4& modelMatrix) override {
        if (!spline.isValid()) return;

    
        glm::vec3 pos = spline.evaluate(t);

        
        glm::vec3 tangent = spline.tangent(t);
        glm::vec3 forward = glm::normalize(tangent);
     
        glm::vec3 worldUp = glm::vec3(0, 1, 0);

     
        glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));

        
        if (glm::length(right) < 0.001f)
            right = glm::vec3(1, 0, 0);

        glm::vec3 up = glm::cross(forward, right);

       
        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0] = glm::vec4(right, 0);
        rotation[1] = glm::vec4(up, 0);
        rotation[2] = glm::vec4(forward, 0);

        
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix *= rotation;
    }

};
