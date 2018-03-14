#include "rocket.h"

class Player : public GameObject
{
public:

    int lives;

    double prev_xPos, prev_yPos;

    virtual ~Player() {}

    virtual void Create()
    {
        name = "player";
        m = PLAYER;
        hitBox_x = 30;
        hitBox_y = 32;
        GameObject::Create();
    }

    virtual void Init()
    {
        SDL_Log("Player::Init");

        horizontalPosition = PLAYER_INIT_X;
        verticalPosition = PLAYER_INIT_Y;

        GameObject::Init();
        lives = NUM_LIVES;

    }

    virtual void Receive(Message m)
    {
        if(m == CENTIPEDE || m == SPIDER || m == FLEA)
        {
            SDL_Log("Player::Hit!");
            lives--;

            if(lives < 0)
                Send(GAME_OVER);
            else
                Send(PLAYER_HIT);
        }
        if(m == MUSHROOM)
        {
            horizontalPosition = prev_xPos;
            verticalPosition = prev_yPos;
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


    virtual void Update(float dt)
    {
        Player * p = (Player *) go;

        p->prev_xPos = p->horizontalPosition;
        p->prev_yPos = p->verticalPosition;

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
                rocket->Init(p->horizontalPosition, p->verticalPosition);
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
