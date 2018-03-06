
class Centipede_segment : public GameObject
{
public:

    int xDir, yDir;
    float speed;
    Centipede_segment * prev;
    bool verticalMovement;
    float vertDist;

    virtual ~Centipede_segment() {}

    virtual void Init(float xPos, float yPos, int xDir, int yDir, float speed, Centipede_segment * prev)
    {
        this->horizontalPosition = xPos;
        this->verticalPosition = yPos;
        this->xDir = xDir;
        this->yDir = yDir;
        this->speed = speed;
        this->verticalMovement = false;
        this->vertDist = 0;
        this->prev = prev;

        GameObject::Init("segment", CENTIPEDE);

    }


    virtual void Receive(Message m)
    {
        if(m == ROCKET)
        {
            SDL_Log("Centipede hit!");
            enabled = false;
            Send(SPLIT_CENTIPEDE);
        }

        if(m == CENTIPEDE)
        {
            SDL_Log("Centipede Collision");
            this->horizontalPosition -= this->xDir;
            vertDist = this->verticalPosition;
            verticalMovement = true;
            this->xDir = -this->xDir;
        }

    }
};

class SegmentBehaviourComponent : public Component
{



public:

    virtual ~SegmentBehaviourComponent() {}



    virtual void Update(float dt)
    {
        Centipede_segment * cent = (Centipede_segment *) go;

        if(cent->prev == NULL)
        {

            if(!cent->verticalMovement)
            {
                cent->horizontalPosition += dt*cent->speed*cent->xDir;
            }
            else if(cent->verticalMovement && abs(cent->verticalPosition-cent->vertDist) <= 32)
            {
                cent->verticalPosition += dt*cent->speed*cent->yDir;
            }
            else{
                cent->vertDist = 0;
                cent->verticalMovement = false;
            }



            if(cent->horizontalPosition > SCREEN_WIDTH-32)
            {
                cent->horizontalPosition = SCREEN_WIDTH-32;
                cent->verticalMovement = true;
                cent->vertDist = cent->verticalPosition;
                cent->xDir = -cent->xDir;
            }

            if(cent->horizontalPosition < 0)
            {
                cent->horizontalPosition = 0;
                cent->verticalMovement = true;
                cent->vertDist = cent->verticalPosition;
                cent->xDir = -cent->xDir;
            }

            if(cent->verticalPosition >= SCREEN_HEIGHT-32)
            {
                cent->verticalPosition = SCREEN_HEIGHT-32;
                cent->yDir = -cent->yDir;
            }

            if(cent->verticalPosition <= 0)
            {
                cent->verticalPosition = 0;
                cent->yDir = -cent->yDir;
            }
        }
        else
        {

            double x = cent->horizontalPosition;
            double y = cent->verticalPosition;
            double prevX = cent->prev->horizontalPosition;
            double prevY = cent->prev->verticalPosition;

            if(cent->verticalMovement)
            {

                if(abs(y-prevY) < 0.1)
                {
                    cent->verticalPosition = prevY;

                    cent->verticalMovement = false;
                }

                else
                {
                    cent->verticalPosition += cent->yDir*cent->speed*dt;

                }
            }
            else
            {

                if(cent->prev->verticalMovement)
                {
                    cent->horizontalPosition += cent->xDir*cent->speed*dt;

                }
                else if(!cent->prev->verticalMovement && y != prevY)
                {
                    cent->yDir = cent->prev->yDir;
                    cent->verticalMovement = true;

                }
                else
                {
                    cent->xDir = cent->prev->xDir;
                    cent->horizontalPosition = prevX-cent->xDir*32;
                }
            }
        }
    }





};
