
class Rocket : public GameObject
{
public:

    virtual void Create()
    {

        name = "rocket";
        m = ROCKET;
        hitBox_x = 4;
        hitBox_y = 18;
        GameObject::Create();
    }

    virtual void Init(double xPos, double yPos)
    {
        SDL_Log("Rocket::Init");
        GameObject::Init();

        horizontalPosition = xPos;
        verticalPosition = yPos;

    }

    virtual void Receive(Message m)
    {
        if(m == CENTIPEDE || m == MUSHROOM || m == SPIDER)
            enabled = false;
    }

};

class RocketBehaviourComponent : public Component
{
public:

    void Update(float dt)
    {
        go->verticalPosition -= ROCKET_SPEED*dt;

        if(go->verticalPosition < 0)
            go->enabled = false;
    }
};
