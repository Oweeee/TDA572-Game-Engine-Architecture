
class Spider : public GameObject
{
public:

    virtual ~Spider() {}

    virtual void Create()
    {
        name = "spider";
        m = SPIDER;
        hitBox_x = 30;
        hitBox_y = 30;
        GameObject::Create();
    }

    virtual void Receive(Message m)
    {
        if(m == ROCKET)
        {
            enabled = false;
            Send(SPIDER_KILL);
        }


    }
};

class SpiderBehaviourComponent : public Component
{

public:

    int xDir, yDir;

    virtual ~SpiderBehaviourComponent() {}

    virtual void Init()
    {

        int side = rand() % 2;
        if(side == 0)
        {
            go->horizontalPosition = 0;
            xDir = 1;
        }
        else
        {
            go->horizontalPosition = SCREEN_WIDTH-64;
            xDir = -1;
        }

        go->verticalPosition = rand() % (SCREEN_HEIGHT-192) + 64;
        yDir = 1;

    }

    virtual void Update(float dt)
    {

        Spider * spider = (Spider *) go;

        spider->horizontalPosition += xDir*dt*SPIDER_SPEED;
        spider->verticalPosition += yDir*dt*SPIDER_SPEED;

        if(spider->horizontalPosition < 0)
        {
            spider->horizontalPosition = 0;
            xDir = -xDir;
        }

        if(spider->horizontalPosition > SCREEN_WIDTH-64)
        {
            spider->horizontalPosition = SCREEN_WIDTH-64;
            xDir = -xDir;
        }

        if(spider->verticalPosition < 0)
        {
            spider->verticalPosition = 0;
            yDir = -yDir;
        }

        if(spider->verticalPosition > SCREEN_HEIGHT-64)
        {
            spider->verticalPosition = SCREEN_HEIGHT-64;
            yDir = -yDir;
        }



    }

};
