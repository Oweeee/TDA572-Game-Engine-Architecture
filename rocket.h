
class Rocket : public GameObject
{
public:

    virtual void Create()
    {
        SDL_Log("Rocket::Create");
        GameObject::Create();
    }

    virtual void Init(double xPos, double yPos)
    {
        SDL_Log("Rocket::Init");
        GameObject::Init("rocket", ROCKET);

        horizontalPosition = xPos;
        verticalPosition = yPos;
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
