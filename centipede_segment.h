
#include <queue>

class Centipede_segment : public GameObject
{
public:

    int xDir, yDir;
    float speed;
    Centipede_segment * prev;
    bool verticalMovement;
    float vertDist;
    std::queue<float> dirChanges_x;
    std::queue<float> dirChanges_y;

    virtual ~Centipede_segment() {}

    virtual void Create()
    {
        name = "segment";
        m = CENTIPEDE;
        hitBox_x = 28;
        hitBox_y = 28;
        GameObject::Create();
    }

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


        GameObject::Init();

    }


    virtual void Receive(Message m)
    {
        if(m == ROCKET)
        {
            SDL_Log("Centipede hit!");
            enabled = false;
            Send(SPLIT_CENTIPEDE);
        }

        if(m == CENTIPEDE || m == MUSHROOM
           )
        {
            //SDL_Log("Centipede Collision");
            this->horizontalPosition -= this->xDir;
            vertDist = this->verticalPosition;
            verticalMovement = true;
            this->xDir = -this->xDir;
            this->dirChanges_x.push(this->horizontalPosition);
            this->dirChanges_y.push(this->verticalPosition);
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



        /*if(cent->prev == NULL)
        {*/

            if(cent->prev != NULL)
            {
                if(abs(cent->horizontalPosition - cent->prev->horizontalPosition)>32)
                    cent->horizontalPosition = cent->prev->horizontalPosition - cent->xDir*32;

                if(abs(cent->verticalPosition - cent->prev->verticalPosition)>32)
                    cent->verticalPosition = cent->prev->verticalPosition - cent->yDir*32;
            }

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
                cent->dirChanges_x.push(cent->horizontalPosition);
                cent->dirChanges_y.push(cent->verticalPosition);
            }



            if(cent->horizontalPosition > SCREEN_WIDTH-32)
            {

                cent->horizontalPosition = SCREEN_WIDTH-32;
                cent->verticalMovement = true;
                cent->vertDist = cent->verticalPosition;
                cent->xDir = -cent->xDir;
                cent->dirChanges_x.push(cent->horizontalPosition);
                cent->dirChanges_y.push(cent->verticalPosition);
            }

            if(cent->horizontalPosition < 0.f)
            {
                cent->horizontalPosition = 0;
                cent->verticalMovement = true;
                cent->vertDist = cent->verticalPosition;
                cent->xDir = -cent->xDir;
                cent->dirChanges_x.push(cent->horizontalPosition);
                cent->dirChanges_y.push(cent->verticalPosition);
            }

            if(cent->verticalPosition > SCREEN_HEIGHT-32)
            {

                cent->yDir = -cent->yDir;
                cent->verticalPosition = SCREEN_HEIGHT-32;
                cent->vertDist = 1000;

            }

            if(cent->verticalPosition < 0.f)
            {

                cent->yDir = -cent->yDir;
                cent->verticalPosition = 0;
                cent->vertDist = 1000;


            }
        /*}
        else
        {

            double x = cent->horizontalPosition;
            double y = cent->verticalPosition;
            double prevX = cent->prev->horizontalPosition;
            double prevY = cent->prev->verticalPosition;


            if(cent->verticalMovement)
            {

                if(!cent->prev->dirChanges_y.empty() && abs(y-cent->prev->dirChanges_y.front()) < 0.1 && abs(x - cent->prev->dirChanges_x.front())<0.1)
                {
                    cent->horizontalPosition = cent->prev->dirChanges_x.front();
                    cent->verticalPosition = cent->prev->dirChanges_y.front();
                    cent->prev->dirChanges_x.pop();
                    cent->prev->dirChanges_y.pop();

                    cent->xDir = cent->prev->xDir;
                    cent->verticalMovement = false;

                    cent->dirChanges_x.push(cent->horizontalPosition);
                    cent->dirChanges_y.push(cent->verticalPosition);
                }

                else
                {

                    if(abs(y-prevY) > 32){
                        SDL_Log("yep");
                        cent->verticalPosition = prevY - (32*cent->yDir);
                    }



                    cent->verticalPosition += cent->yDir*cent->speed*dt;

                    if(cent->verticalPosition > SCREEN_HEIGHT-32)
                        cent->verticalPosition = SCREEN_HEIGHT-32;

                    if(cent->verticalPosition < 0.f)
                        cent->verticalPosition = 0.f;

                }
            }
            else
            {


                if(!cent->prev->dirChanges_x.empty() && abs(x - cent->prev->dirChanges_x.front())<0.1 && abs(y-cent->prev->dirChanges_y.front()) < 0.1)
                {

                    cent->horizontalPosition = cent->prev->dirChanges_x.front();
                    //cent->verticalPosition = cent->prev->verticalPosition-(cent->prev->yDir*32);

                    cent->prev->dirChanges_x.pop();
                    cent->prev->dirChanges_y.pop();

                    cent->dirChanges_x.push(cent->horizontalPosition);
                    cent->dirChanges_y.push(cent->verticalPosition);



                    cent->yDir = cent->prev->yDir;
                    cent->verticalMovement = true;



                }
                else if(x!=prevX)
                {
                    //SDL_Log("1");
                    cent->horizontalPosition += cent->xDir*cent->speed*dt;

                    if(cent->horizontalPosition > SCREEN_WIDTH-32)
                        cent->horizontalPosition = SCREEN_WIDTH-32;

                    if(cent->horizontalPosition < 0.f)
                        cent->horizontalPosition = 0.f;

                }
                else
                {
                    //'->xDir = cent->prev->xDir;
                    cent->horizontalPosition = prevX-cent->xDir*32;
                }
            }
        }*/
    }





};
