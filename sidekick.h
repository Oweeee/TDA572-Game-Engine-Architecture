#ifndef SIDEKICK.H
#define SIDEKICK.H

#include "mushroom.h"

class Sidekick : public GameObject
{
public:

    virtual ~Sidekick() {}

    virtual void Init()
    {

        horizontalPosition = PLAYER_INIT_X-64;
        verticalPosition = PLAYER_INIT_Y;

        GameObject::Init();
    }
};

class SidekickBehaviourComponent : public Component
{

    Rocket * rocket;
    std::set<Mushroom*> * mushrooms;
    Mushroom* target;
    bool inPosition;


public:
    virtual ~SidekickBehaviourComponent() {}

    virtual void Create(AvancezLib* system, GameObject* go, std::set<GameObject*>* game_objects, Rocket * rocket, std::set<Mushroom*> * mushrooms)
    {
        Component::Create(system, go, game_objects);
        this->rocket = rocket;
        this->mushrooms = mushrooms;
        target = NULL;
        inPosition = false;

    }

    virtual void Update(float dt)
    {


        if(target != NULL && !target->enabled)
        {
            inPosition = false;
            target = NULL;
        }


        if(target != NULL && inPosition)
            Fire();

        else if(target != NULL)
            Move(target, dt);

        else
            target = findTarget(mushrooms);

    }

    Mushroom* findTarget(std::set<Mushroom*> * mushrooms)
    {

        SDL_Log("find target");

        Mushroom* mush = NULL;

        float lowest = 0;
        float closest = 1000;

        for(auto it = mushrooms->begin(); it != mushrooms->end(); it++)
        {
            if((*it)->enabled)
            {
                if((*it)->verticalPosition > lowest)
                {

                    mush = (*it);
                    lowest = (*it)->verticalPosition;
                    closest = abs((*it)->horizontalPosition - go->horizontalPosition);


                }
                if((*it)->verticalPosition == lowest && abs((*it)->horizontalPosition - go->horizontalPosition) < closest)
                {
                    mush = (*it);
                    closest = abs((*it)->horizontalPosition - go->horizontalPosition);
                }

            }


        }
        return mush;

    }

    void Fire()
    {
        SDL_Log("fire");

        if(!rocket->enabled)
        {
            rocket->Init(go->horizontalPosition, go->verticalPosition);
            game_objects->insert(rocket);
        }
    }

    void Move(Mushroom* target, float dt)
    {

        //SDL_Log("move");

        float firePos = target->verticalPosition + 64;

        if(abs(go->verticalPosition - firePos) < 1 && abs(go->horizontalPosition - target->horizontalPosition) < 1)
            inPosition = true;


        if(abs(go->verticalPosition - firePos) >= abs(go->horizontalPosition - target->horizontalPosition))
        {
            if(go->verticalPosition > firePos)
                go->verticalPosition -= dt*SIDEKICK_SPEED;
            else
                go->verticalPosition += dt*SIDEKICK_SPEED;
        }

        else if(go->verticalPosition == firePos)
        {
            if(go->horizontalPosition > target->horizontalPosition)
                go->horizontalPosition -= dt*SIDEKICK_SPEED;
            else
                go->horizontalPosition += dt*SIDEKICK_SPEED;
        }
        else
        {
            if(go->verticalPosition > firePos)
            {
                if(go->horizontalPosition > target->horizontalPosition)
                {
                    go->verticalPosition -= dt*SIDEKICK_SPEED;
                    go->horizontalPosition -= dt*SIDEKICK_SPEED;
                }

                else
                {
                    go->verticalPosition -= dt*SIDEKICK_SPEED;
                    go->horizontalPosition += dt*SIDEKICK_SPEED;
                }

            }

            else
            {
                if(go->horizontalPosition > target->horizontalPosition)
                {
                    go->verticalPosition += dt*SIDEKICK_SPEED;
                    go->horizontalPosition -= dt*SIDEKICK_SPEED;
                }

                else
                {
                    go->verticalPosition += dt*SIDEKICK_SPEED;
                    go->horizontalPosition += dt*SIDEKICK_SPEED;
                }
            }


        }
    }
};

#endif
