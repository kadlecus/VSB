#pragma once
#include <glm/glm.hpp>

class Material {
public:
    float ra;  
    float rd;   
    float rs;   
    float shininess;

    Material(
        float ra = 0.1f,
        float rd = 1.0f,
        float rs = 1.0f,
        float shininess = 32.0f
    )
        : ra(ra), rd(rd), rs(rs), shininess(shininess) {}
};
