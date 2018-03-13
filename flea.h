
class Flea : public GameObject
{
public:

    virtual ~Flea() {}

    virtual void Create()
    {
        name = "flea";
        m = FLEA;
        hitBox_x = 30;
        hitBox_y = 30;
        GameObject::Create();
    }

    virtual void Receive(Message m)
    {
        if(m == ROCKET)
        {
            enabled = false;
            Send(FLEA_KILL);
        }
    }

};

class FleaBehaviourComponent : public Component
{

public:

    virtual ~FleaBehaviourComponent() {}

    virtual void Init()
    {
        int range = (SCREEN_WIDTH - 96)/32;

        go->horizontalPosition = (rand() % range)*32 + 48;
        go->verticalPosition = 0;

    }

    virtual void Update(float dt)
    {

        Flea * flea = (Flea *) go;

        flea->verticalPosition += FLEA_SPEED*dt;

        if(flea->verticalPosition > SCREEN_HEIGHT - 32)
            flea->enabled = false;


        if(flea->verticalPosition >= 64 &&
           flea->verticalPosition < SCREEN_HEIGHT-96 &&
           (int)flea->verticalPosition % 32 == 0)
        {
            flea->Send(MUSHROOM_INIT);
        }


    }
};
