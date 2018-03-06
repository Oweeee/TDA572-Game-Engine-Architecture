
#include "centipede_segment.h"
#include <vector>


class Centipede : public GameObject
{
public:
    std::vector<Centipede_segment*> segments;

    int length, xDir, yDir;

    float speed;

public:



	virtual ~Centipede() { SDL_Log("Centipede::~Centipede"); }

	virtual void Init(int length, float xPos, float yPos, int xDir, int yDir, float speed)
	{


	    this->length = length;

	    this->horizontalPosition = xPos;
	    this->verticalPosition = yPos;
	    this->xDir = xDir;
	    this->yDir = yDir;
	    this->speed = speed;

		SDL_Log("Centipede::Init");
		GameObject::Init("centipede", CENTIPEDE);
	}

	void Disable()
	{
	    for(int i = 0; i < length; i++)
        {
            segments[i]->enabled = false;
        }
        enabled = false;
	}

};

class CentipedeBehaviourComponent : public Component
{


    ObjectPool<Centipede_segment> * body_segment_pool;

    ObjectPool<Centipede_segment> * head_segment_pool;

    Centipede * centipede;



public:

    virtual ~CentipedeBehaviourComponent() {}



    virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects,
                        ObjectPool<Centipede_segment> * head_segment_pool, ObjectPool<Centipede_segment> * body_segment_pool)
    {
        Component::Create(system, go, game_objects);

        this->head_segment_pool = head_segment_pool;

        this->body_segment_pool = body_segment_pool;

        centipede = (Centipede *) go;
    }

    virtual void Init()
    {

        int length = centipede->length;
        float xPos, yPos;

        if(centipede->horizontalPosition > SCREEN_WIDTH-32)
            xPos = SCREEN_WIDTH-32;
        else if(centipede->horizontalPosition < 0)
            xPos = 0;
        else
        {
            if((int)centipede->horizontalPosition % 32 < 16)
                xPos = floor(centipede->horizontalPosition/32)*32;
            else
                xPos = ceil(centipede->horizontalPosition/32)*32;
        }


        if(centipede->verticalPosition > SCREEN_HEIGHT-32)
            yPos = SCREEN_HEIGHT-32;
        else if(centipede->verticalPosition < 0)
            yPos = 0;
        else
        {
            if((int)centipede->verticalPosition % 32 < 16)
                yPos = floor(centipede->verticalPosition/32)*32;
            else
                yPos = ceil(centipede->verticalPosition/32)*32;
        }


        int xDir = centipede->xDir;
        int yDir = centipede->yDir;
        float speed = centipede->speed;

        centipede->segments.reserve(length);

        Centipede_segment * head = head_segment_pool->FirstAvailable();

        SDL_Log("Head Init, x = %f, y = %f", xPos, yPos);
        head->Init(xPos, yPos, xDir, yDir, speed, NULL);
        centipede->segments[0] = head;


        for(int i = 1; i < length; i++)
        {
            Centipede_segment * segment = body_segment_pool->FirstAvailable();
            Centipede_segment * prev = centipede->segments[i-1];
            float segment_xPos = prev->horizontalPosition-(xDir*32);         //xPos-(xDir*i*32);
            float segment_yPos = prev->verticalPosition;
            int segment_xDir = prev->xDir;
            int segment_yDir = prev->yDir;
            if(segment_xPos < 0)
            {
                segment_yPos -=segment_yDir*32;
                segment_xPos = abs(segment_xPos);
                segment_xDir = -segment_xDir;
            }
            else if(segment_xPos > SCREEN_WIDTH-32)
            {
                segment_yPos -=segment_yDir*32;
                segment_xPos = SCREEN_WIDTH-(segment_xPos - SCREEN_WIDTH)-32;
                segment_xDir = -segment_xDir;
            }
            SDL_Log("Segment %i Init, x = %f  y = %f", i, segment_xPos, segment_yPos);
            segment->Init(segment_xPos, segment_yPos, segment_xDir, segment_yDir, speed, prev);
            centipede->segments[i] = segment;
        }

    }

    virtual void Update(float dt)
    {

        centipede->segments[0]->Update(dt);

        for(int i = 1; i <  centipede->length; i++)
        {
            centipede->segments[i]->Update(dt);
        }
        //SDL_Log("head Y - tail Y = %d", abs(centipede->segments[0]->verticalPosition - centipede->segments[centipede->length-1]->verticalPosition));
    }
};

