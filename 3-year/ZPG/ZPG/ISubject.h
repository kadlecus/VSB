#pragma once
#include <vector>
#include "IObserver.h"
#include <iostream>
class ISubject {
protected:
    std::vector<IObserver*> observers;

public:
    virtual ~ISubject() = default;

    void attach(IObserver* observer) {
        observers.push_back(observer);
    }

    void detach(IObserver* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

public:
    void notify() {
        for (auto observer : observers) {
            if (observer) observer->update(this);
        }
        //std::cout << "[LightManager::notify] observers = " << observers.size() << "\n";
    }
};
