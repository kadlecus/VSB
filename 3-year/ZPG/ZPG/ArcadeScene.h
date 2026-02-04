#pragma once
#include "Scene.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Models/gift.h"
#include "PathMove.h"
#include "Translate.h"

class ArcadeScene : public Scene
{
private:

    enum ObjectType {
        Mole,
        BonusMole
    };

    struct MoleInfo {
        int id;
        float timeLeft;
        ObjectType type;
    };

    std::vector<MoleInfo> moles;
    float spawnInterval = 1.0f;
    float spawnTimer = 0.0f;
    float objectLifetime = 5.0f;

    int score = 0;

public:

    void update(float dt, Camera* camera) override
    {
        Scene::update(dt, camera);

        spawnTimer -= dt;

        if (spawnTimer <= 0.0f)
        {
            spawnRandomObject();
            spawnTimer = spawnInterval;
        }

       
        for (auto it = moles.begin(); it != moles.end(); )
        {
            it->timeLeft -= dt;

            if (it->timeLeft <= 0.0f)
            {
                removeObjectById(it->id);
                it = moles.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void spawnRandomObject()
    {
        ShaderProgram* shader = getShaderProgram(0);

        float r = static_cast<float>(rand()) / RAND_MAX;
        ObjectType type = (r < 0.2f) ? BonusMole : Mole;

        float x = ((rand() % 1000) / 1000.0f) * 10.0f - 5.0f;
        float y = ((rand() % 1000) / 1000.0f) * 10.0f - 5.0f;
        float z = ((rand() % 1000) / 1000.0f) * 10.0f - 5.0f;

        glm::vec3 startPos(x, y, z);

        auto obj = addObjectAt(gift, sizeof(gift), 6, startPos, shader);
        int id = obj->getId();

        if (type == BonusMole)
            obj->setColor(glm::vec3(1.0f, 1.0f, 0.0f));

     
        std::vector<glm::vec3> path;
        path.push_back(startPos);

        for (int i = 0; i < 2; i++)
        {
            float nx = startPos.x + ((rand() % 200) - 100) / 15.0f;
            float ny = startPos.y + ((rand() % 200) - 100) / 15.0f;
            float nz = startPos.z + ((rand() % 200) - 100) / 15.0f;
            path.push_back(glm::vec3(nx, ny, nz));
        }

       
        if (auto trans = obj->getTransformation())
        {
            trans->addComponent(std::make_shared<PathMove>(path, 0.4f));
        }

        moles.push_back({ id, objectLifetime, type });

        std::cout << "Spawned object ID " << id
            << " Type: " << ((type == Mole) ? "Mole" : "BonusMole")
            << " Score: " << score << std::endl;
    }

    void onObjectClicked(int objectId)
    {
        for (auto it = moles.begin(); it != moles.end(); ++it)
        {
            if (it->id == objectId)
            {
                score += (it->type == Mole) ? 1 : 5;

                removeObjectById(objectId);
                moles.erase(it);

                std::cout << "Hit object ID " << objectId
                    << " New Score: " << score << std::endl;
                break;
            }
        }
    }
};
