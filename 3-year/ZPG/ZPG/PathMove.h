#pragma once
#include "TransformationComponent.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class PathMove : public TransformationComponent
{
private:
    std::vector<glm::vec3> points; 
    int currentSegment = 0;        
    float t = 0.0f;                
    float speed = 0.3f;             

public:
    PathMove(const std::vector<glm::vec3>& pts, float speed)
        : points(pts), speed(speed)
    {
    }

    void update(float dt) override
    {
        if (points.size() < 2) return;

        t += speed * dt;

        if (t >= 1.0f)
        {
            t = 0.0f;
            currentSegment++;
            if (currentSegment >= points.size() - 1)
            {
                currentSegment = 0;   
            }
        }
    }

    void apply(glm::mat4& modelMatrix) override
    {
        if (points.size() < 2) return;

        glm::vec3 p0 = points[currentSegment];
        glm::vec3 p1 = points[currentSegment + 1];

        glm::vec3 pos = (1.0f - t) * p0 + t * p1;

        modelMatrix = glm::translate(modelMatrix, pos);
    }
};
