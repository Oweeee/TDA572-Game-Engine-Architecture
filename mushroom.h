
class Mushroom : public GameObject
{
public:

    virtual ~Mushroom() {}

    virtual void Create()
    {
        name = "mushroom";
        m = MUSHROOM;
        hitBox_x = 30;
        hitBox_y = 30;
        GameObject::Create();
    }

    virtual void Init(double xPos, double yPos)
    {

        horizontalPosition = xPos;
        verticalPosition = yPos;


        GameObject::Init();

    }

    virtual void Receive(Message m)
    {
        if(m == ROCKET)
        {
            enabled = false;
            Send(MUSHROOM_DESTROY);
        }

    }

};
