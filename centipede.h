
#include "centipede_segment.h"
#include <vector>


class Centipede : public GameObject
{
public:
    std::vector<Centipede_segment*> segments;

    int length, xDir, yDir;

    float speed;

    ObjectPool<Centipede_segment> * body_segment_pool;

    ObjectPool<Centipede_segment> * head_segment_pool;

	virtual ~Centipede() { SDL_Log("Centipede::~Centipede"); }


	virtual void Create(ObjectPool<Centipede_segment> * head_segment_pool, ObjectPool<Centipede_segment> * body_segment_pool)
	{

        this->head_segment_pool = head_segment_pool;

        this->body_segment_pool = body_segment_pool;


        name = "centipede";
	    m = CENTIPEDE;
	    GameObject::Create();
	}

	//Init a new Centipede with part of an old Centipede as reference.
	virtual void Init(std::vector<Centipede_segment> segments)
	{


	    this->length = segments.size();
	    this->horizontalPosition = segments[0].horizontalPosition;
	    this->verticalPosition = segments[0].verticalPosition;
	    this->xDir = segments[0].xDir;
	    this->yDir = segments[0].yDir;
	    this->speed = segments[0].speed;


	    this->segments.reserve(segments.size());


	    Centipede_segment * head = head_segment_pool->FirstAvailable();

        head->Init(horizontalPosition, verticalPosition, xDir, yDir, speed, NULL);

        head->dirChanges_x = segments[0].dirChanges_x;
        head->dirChanges_y = segments[0].dirChanges_y;

        if(segments[0].verticalMovement){
            head->verticalMovement = true;
            head->vertDist = segments[0].vertDist;
        }

        this->segments.push_back(head);


        for(int i = 1; i < segments.size(); i++)
        {
            Centipede_segment * segment = body_segment_pool->FirstAvailable();
            Centipede_segment * prev = this->segments[i-1];

            segment->Init(segments[i].horizontalPosition, segments[i].verticalPosition, segments[i].xDir, segments[i].yDir, speed, prev);

            segment->dirChanges_x = segments[i].dirChanges_x;
            segment->dirChanges_y = segments[i].dirChanges_y;

            if(segments[i].verticalMovement){
                segment->verticalMovement = true;
                segment->vertDist = segments[i].vertDist;
            }



            this->segments.push_back(segment);
        }


		SDL_Log("Centipede::Init");
		GameObject::Init();

	}


	//Init a new Centipede with a position as offset.
	virtual void Init(int length, float xPos, float yPos, int xDir, int yDir, float speed)
	{


	    this->length = length;

	    this->horizontalPosition = xPos;
	    this->verticalPosition = yPos;
	    this->xDir = xDir;
	    this->yDir = yDir;
	    this->speed = speed;
        segments.reserve(length);


	    //Init Head

	    //Making sure the head is not initiated at weird positions.
        if(horizontalPosition > SCREEN_WIDTH-32)
            horizontalPosition = SCREEN_WIDTH-32;
        else if(horizontalPosition < 0)
            horizontalPosition = 0;
        else
        {
            if((int)horizontalPosition % 32 < 16)
                horizontalPosition = floor(horizontalPosition/32)*32;
            else
                horizontalPosition = ceil(horizontalPosition/32)*32;
        }

        if(verticalPosition > SCREEN_HEIGHT-32)
            verticalPosition = SCREEN_HEIGHT-32;
        else if(verticalPosition < 0)
            verticalPosition = 0;
        else
        {
            if(((int)verticalPosition) % 32 < 16)
                verticalPosition = floor(verticalPosition/32)*32;
            else
                verticalPosition = ceil(verticalPosition/32)*32;
        }

        Centipede_segment * head = head_segment_pool->FirstAvailable();

        head->Init(horizontalPosition, verticalPosition, xDir, yDir, speed, NULL);
        segments.push_back(head);

        SDL_Log("head xPos = %f", head->horizontalPosition);
        SDL_Log("segments[0] xPos = %f", segments[0]->horizontalPosition);


        //Init a body of segments.

        for(int i = 1; i < length; i++)
        {
            Centipede_segment * segment = body_segment_pool->FirstAvailable();
            Centipede_segment * prev = segments[i-1];
            float segment_xPos = prev->horizontalPosition-(xDir*32);         //xPos-(xDir*i*32);
            float segment_yPos = prev->verticalPosition;
            int segment_xDir = prev->xDir;
            int segment_yDir = prev->yDir;

            if((int)segment_xPos < 0)
            {
                segment_yPos -=segment_yDir*32;
                segment_xPos = abs(segment_xPos);
                segment_xDir = -segment_xDir;
            }

            else if((int)segment_xPos > SCREEN_WIDTH-32)
            {
                segment_yPos -=segment_yDir*32;
                segment_xPos = SCREEN_WIDTH-(segment_xPos - SCREEN_WIDTH)-32;
                segment_xDir = -segment_xDir;
            }

            segment->Init(segment_xPos, segment_yPos, segment_xDir, segment_yDir, speed, prev);
            segments.push_back(segment);
        }



		SDL_Log("Centipede::Init");
		GameObject::Init();
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

    Centipede * centipede;

public:

    virtual ~CentipedeBehaviourComponent() {}



    virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects)
    {
        Component::Create(system, go, game_objects);

        centipede = (Centipede *) go;
    }

    virtual void Update(float dt)
    {

        centipede->segments[0]->Update(dt);

        for(int i = 1; i <  centipede->length; i++)
        {
            centipede->segments[i]->Update(dt);
        }
    }
};

