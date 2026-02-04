#pragma once
#include <glm/glm.hpp>
#include <vector>

class BezierSpline {
public:
    std::vector<glm::vec3> points;  

    BezierSpline(const std::vector<glm::vec3>& pts) : points(pts) {}


    glm::vec3 evaluate(float t) const {
        if (points.size() < 4)
            return glm::vec3(0);

        int segmentCount = (points.size() - 1) / 3;

        float fullT = t * segmentCount;
        int seg = glm::floor(fullT);

        if (seg >= segmentCount) seg = segmentCount - 1;

        float localT = fullT - seg;

        int i = seg * 3;
        glm::vec3 P0 = points[i];
        glm::vec3 P1 = points[i + 1];
        glm::vec3 P2 = points[i + 2];
        glm::vec3 P3 = points[i + 3];

        float u = 1.0f - localT;

        return
            u * u * u * P0 +
            3 * u * u * localT * P1 +
            3 * u * localT * localT * P2 +
            localT * localT * localT * P3;
    }

    int getSegmentCount() const {
        return (points.size() - 1) / 3;
    }

    bool isValid() const {
        return points.size() >= 4;
    }

    glm::vec3 tangent(float t) const {
        if (points.size() < 4)
            return glm::vec3(1, 0, 0);

        int segmentCount = (points.size() - 1) / 3;

        float fullT = t * segmentCount;
        int seg = glm::floor(fullT);

        if (seg >= segmentCount) seg = segmentCount - 1;

        float localT = fullT - seg;
        float u = 1.0f - localT;

        int i = seg * 3;
        glm::vec3 P0 = points[i];
        glm::vec3 P1 = points[i + 1];
        glm::vec3 P2 = points[i + 2];
        glm::vec3 P3 = points[i + 3];

      
        glm::vec3 d =
            (-3.f * u * u) * P0 +
            (3.f * u * u - 6.f * u * localT) * P1 +
            (6.f * u * localT - 3.f * localT * localT) * P2 +
            (3.f * localT * localT) * P3;

        return d;
    }

};
