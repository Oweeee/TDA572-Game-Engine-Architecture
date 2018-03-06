#include "rocket.h"

class Player : public GameObject
{
public:

    int lives;

    virtual ~Player() {}

    virtual void Init()
    {
        SDL_Log("Player::Init");
        GameObject::Init("player", PLAYER);
        lives = NUM_LIVES;
    }

    virtual void Receive(Message m)
    {
        SDL_Log("Player Receive");
        if(m == CENTIPEDE)
        {
            SDL_Log("Player::Hit!");
            lives--;

            if(lives == 0)
                Send(GAME_OVER);
            else
                Send(PLAYER_HIT);
        }

    }
};

class PlayerBehaviourComponent : public Component
{

    Rocket * rocket;

public:

    virtual ~PlayerBehaviourComponent() {}

    virtual void Create(AvancezLib* system, GameObject* go, std::set<GameObject*>* game_objects, Rocket * rocket)
    {
        Component::Create(system, go, game_objects);
        this->rocket = rocket;
    }

    virtual void Init()
    {
        go->horizontalPosition = PLAYER_INIT_X;
        go->verticalPosition = PLAYER_INIT_Y;
    }

    virtual void Update(float dt)
    {
        AvancezLib::KeyStatus keys;
        system->getKeyStatus(keys);
        if(keys.right) Move('x', dt * PLAYER_SPEED);
        if(keys.left)  Move('x', dt * -PLAYER_SPEED);
        if(keys.up)    Move('y', dt * -PLAYER_SPEED);
        if(keys.down)  Move('y', dt * PLAYER_SPEED);

        if(keys.fire)
        {

            if(rocket != NULL && !rocket->enabled)
            {
                rocket->Init(go->horizontalPosition, go->verticalPosition);
                game_objects->insert(rocket);

            }
        }
    }

    void Move(char axis, float speed)
    {
        if(axis == 'x')
        {
            go->horizontalPosition += speed;

            if (go->horizontalPosition > (SCREEN_WIDTH - 32))
                go->horizontalPosition = SCREEN_WIDTH - 32;

            if (go->horizontalPosition < 0)
                go->horizontalPosition = 0;
        }
        else if(axis == 'y')
        {
            go->verticalPosition += speed;

            if (go->verticalPosition > (SCREEN_HEIGHT - 32))
                go->verticalPosition = SCREEN_HEIGHT - 32;

            if (go->verticalPosition < 0)
                go->verticalPosition = 0;
        }
    }

};
