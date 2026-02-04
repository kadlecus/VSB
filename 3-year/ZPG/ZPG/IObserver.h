#pragma once
#include <vector>
#include <memory>


class Camera;
class Light;
class ISubject;

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(ISubject* subject) = 0;
    //virtual void updateFromCamera(Camera* camera) {};
    //virtual void updateFromLight(Light* light) {}
    //virtual void updateFromLights(const std::vector<std::shared_ptr<Light>>& lights) {};
}; 