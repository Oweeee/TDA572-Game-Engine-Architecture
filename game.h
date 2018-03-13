#include <ctime>


class Game : public GameObject
{

    std::set<GameObject*> game_objects;
    std::set<Mushroom*> mushrooms;

    std::deque<Centipede*> centipedes;

    AvancezLib* system;

    Player * player;

    Rocket * rocket;

    ObjectPool<Centipede_segment> head_pool;

    ObjectPool<Centipede_segment> body_pool;

    ObjectPool<Centipede> centipede_pool;

    ObjectPool<Mushroom> mushroom_pool;

    ObjectPool<Spider> spider_pool;

    Sprite * life_sprite;

    bool game_over;

    unsigned int score = 0;

    unsigned int gameIteration;

    int spider_cd;

public:

    virtual void Create(AvancezLib* system)
    {
        SDL_Log("Game::Create");

        this->system = system;


        rocket = new Rocket();
        RocketBehaviourComponent * rocket_behaviour = new RocketBehaviourComponent();
        rocket_behaviour->Create(system, rocket, &game_objects);
        RenderComponent * rocket_render = new RenderComponent();
        rocket_render->Create(system, rocket, &game_objects, "data/rocket.bmp");

        CollideComponent * head_collider = new CollideComponent();
        head_collider->Create(system, rocket, &game_objects, (ObjectPool<GameObject>*) &head_pool);
        CollideComponent * body_collider = new CollideComponent();
        body_collider->Create(system, rocket, &game_objects, (ObjectPool<GameObject>*) &body_pool);
        CollideComponent * mushroom_collider = new CollideComponent();
        mushroom_collider->Create(system, rocket, &game_objects, (ObjectPool<GameObject>*) &mushroom_pool);
        CollideComponent * spider_collider = new CollideComponent();
        spider_collider->Create(system, rocket, &game_objects, (ObjectPool<GameObject>*) &spider_pool);


        rocket->Create();
        rocket->AddComponent(rocket_behaviour);
        rocket->AddComponent(rocket_render);
        rocket->AddComponent(head_collider);
        rocket->AddComponent(body_collider);
        rocket->AddComponent(mushroom_collider);
        rocket->AddComponent(spider_collider);
        rocket->AddReceiver(this);

        player = new Player();
        PlayerBehaviourComponent* player_behaviour = new PlayerBehaviourComponent();
        player_behaviour->Create(system, player, &game_objects, rocket);
        RenderComponent* player_render = new RenderComponent();
        player_render->Create(system, player, &game_objects, "data/player_2.bmp");
        CollideComponent * player_headCollider = new CollideComponent();
        player_headCollider->Create(system, player, &game_objects, (ObjectPool<GameObject>*) &head_pool);
        CollideComponent * player_bodyCollider = new CollideComponent();
        player_bodyCollider->Create(system, player, &game_objects, (ObjectPool<GameObject>*) &body_pool);
        CollideComponent * player_mushroomCollider = new CollideComponent();
        player_mushroomCollider->Create(system, player, &game_objects, (ObjectPool<GameObject>*) &mushroom_pool);
        CollideComponent * player_spiderCollider = new CollideComponent();
        player_spiderCollider->Create(system, player, &game_objects, (ObjectPool<GameObject>*) &spider_pool);


        player->Create();
        player->AddComponent(player_behaviour);
        player->AddComponent(player_render);
        player->AddComponent(player_headCollider);
        player->AddComponent(player_bodyCollider);
        player->AddComponent(player_mushroomCollider);
        player->AddComponent(player_spiderCollider);
        player->AddReceiver(this);
        game_objects.insert(player);

        life_sprite = system->createSprite("data/player_2.bmp");


        head_pool.Create(20);
        for(auto head = head_pool.pool.begin(); head != head_pool.pool.end(); head++)
        {
            SegmentBehaviourComponent * head_behaviour = new SegmentBehaviourComponent();
            head_behaviour->Create(system, *head, &game_objects);
            RenderComponent * head_render = new RenderComponent();
            head_render->Create(system, *head, &game_objects, "data/head/centipede_head_right_1.bmp");
            CollideComponent * head_collider = new CollideComponent();
            head_collider->Create(system, *head, &game_objects, (ObjectPool<GameObject>*) &head_pool);
            CollideComponent * mush_collider = new CollideComponent();
            mush_collider->Create(system, *head, &game_objects, (ObjectPool<GameObject>*) &mushroom_pool);


            (*head)->Create();
            (*head)->AddComponent(head_behaviour);
            (*head)->AddComponent(head_render);
            (*head)->AddComponent(head_collider);
            (*head)->AddComponent(mush_collider);
            (*head)->AddReceiver(this);
        }

        body_pool.Create(20);
        for(auto body = body_pool.pool.begin(); body != body_pool.pool.end(); body++)
        {
            SegmentBehaviourComponent * body_behaviour = new SegmentBehaviourComponent();
            body_behaviour->Create(system, *body, &game_objects);
            RenderComponent * body_render = new RenderComponent();
            body_render->Create(system, *body, &game_objects, "data/body/centipede_body_right_1.bmp");
            CollideComponent * mush_collider = new CollideComponent();
            mush_collider->Create(system, *body, &game_objects, (ObjectPool<GameObject>*) &mushroom_pool);

            (*body)->Create();
            (*body)->AddComponent(body_behaviour);
            (*body)->AddComponent(body_render);
            (*body)->AddComponent(mush_collider);
            (*body)->AddReceiver(this);
        }

        centipede_pool.Create(20);

        for(auto centipede = centipede_pool.pool.begin(); centipede != centipede_pool.pool.end(); centipede++)
        {
            CentipedeBehaviourComponent * centipede_behaviour = new CentipedeBehaviourComponent();
            centipede_behaviour->Create(system, *centipede, &game_objects);

            (*centipede)->Create(&head_pool, &body_pool);
            (*centipede)->AddComponent(centipede_behaviour);
            (*centipede)->AddReceiver(this);
            game_objects.insert(*centipede);

        }

        spider_pool.Create(10);

        for(auto spider = spider_pool.pool.begin(); spider != spider_pool.pool.end(); spider++)
        {
            SpiderBehaviourComponent * spider_behaviour = new SpiderBehaviourComponent();
            spider_behaviour->Create(system, *spider, &game_objects);
            RenderComponent * spider_render = new RenderComponent();
            spider_render->Create(system, *spider, &game_objects, "data/spider.bmp");

            (*spider)->Create();
            (*spider)->AddComponent(spider_behaviour);
            (*spider)->AddComponent(spider_render);
            (*spider)->AddReceiver(this);
            game_objects.insert(*spider);

        }

        mushroom_pool.Create(300);

        for(auto mushroom = mushroom_pool.pool.begin(); mushroom != mushroom_pool.pool.end(); mushroom++)
        {
            RenderComponent * mushroom_render = new RenderComponent();
            mushroom_render->Create(system, *mushroom, &game_objects, "data/mushroom.bmp");

            (*mushroom)->Create();
            (*mushroom)->AddComponent(mushroom_render);
            (*mushroom)->AddReceiver(this);
            mushrooms.insert(*mushroom);
        }
    }

    virtual void Init()
    {

        gameIteration = 1;

        player->Init();

        InitMushrooms(50);

        spider_cd = rand() % 20 + 10;

        Centipede * centipede = centipede_pool.FirstAvailable();
        //centipede->Init(10, 32, 32.f, -1, -1, 200);
        centipedes.push_front(centipede);

        enabled = true;
        game_over = false;

    }

    virtual void Update(float dt)
    {

        if(!IsGameOver())
        {
            if(centipedes.empty())
                NewGame(gameIteration);

            //Update mushrooms before the rest in order to draw them behind stuff.
            for(auto mush = mushrooms.begin(); mush != mushrooms.end(); mush++)
                (*mush)->Update(dt);

            if(system->getElapsedTime() >= spider_cd)
                InitSpider();

            for(auto go = game_objects.begin(); go != game_objects.end(); go++)
                (*go)->Update(dt);

        }
        else
            dt = 0.f;



    }

    virtual void Draw()
    {
        char msg[1024];
        sprintf(msg, "%05d", Score());
        system->drawText(0, 10, msg);
        sprintf(msg, "16543");
		system->drawText(300, 10, msg);

		for (int i = 0; i < player->lives; i++)
			life_sprite->draw(i*30+140, 5);

		if (IsGameOver())
		{

			sprintf(msg, "GAME OVER");
			system->drawText(250, SCREEN_HEIGHT/2, msg);
		}
    }

    virtual void Receive(Message m)
    {
        if(m == GAME_OVER)
            game_over = true;

        else if(m == PLAYER_HIT)
            NewGame(gameIteration - 1);

        else if(m == SPLIT_CENTIPEDE)
        {

            IncreaseScore(100);

            bool centFound = false;
            int centIndex = 0;
            for(auto it = centipedes.begin(); it != centipedes.end(); it++)
            {
                for(int i = 0; i < (*it)->length; i++)
                {
                    if(!(*it)->segments[i]->enabled)
                    {
                        centFound = true;
                        centipedes.erase(centipedes.begin() + centIndex);
                        SplitCentipede((*it), i);
                        break;
                    }
                }
                if(centFound)
                    break;

                centIndex++;
            }
        }

        else if(m == MUSHROOM_DESTROY)
            IncreaseScore(1);

        else if(m == SPIDER_KILL)
            IncreaseScore(300);


    }

    void SplitCentipede(Centipede * centipede, int index)
    {

        std::vector<Centipede_segment> segments;

        segments.reserve(centipede->segments.size());

        for(int i = 0; i < centipede->segments.size(); i++)
            segments.push_back(*(centipede->segments[i]));

        for(int i = 0; i < segments.size(); i++)
            SDL_Log("Segment %i xPos = %f", i, segments[i].horizontalPosition);


        centipede->Disable();
        //centipede->Destroy();


        Centipede * newCentipede1 = centipede_pool.FirstAvailable();
        std::vector<Centipede_segment> newSegments1;

        if(index == centipede->length-1)
        {

            if(centipede->length > 1)
            {
                newSegments1.reserve(centipede->length-1);

                for(int i = 0; i < centipede->length-1; i++)
                {

                    newSegments1.push_back(segments[i]);
                }


                newCentipede1->Init(newSegments1);
                centipedes.push_front(newCentipede1);
            }

        }
        else
        {

            if(index == 0)
            {
                newSegments1.reserve(centipede->length-1);

                for(int i = 0; i < centipede->length-1; i++)
                {
                    segments[centipede->length-1-i].xDir *=-1;
                    newSegments1.push_back(segments[centipede->length-1-i]);
                }


                newCentipede1->Init(newSegments1);
                SDL_Log("newCentipede1 length = %i", newCentipede1->length);
                centipedes.push_front(newCentipede1);
            }
            else
            {

                newSegments1.reserve(centipede->length-(index+1));

                for(int i = 0; i < centipede->length-(index+1); i++)
                {
                    SDL_Log("cent1 segment %i xPos = %f, yPos = %f", i, segments[centipede->length-1-i].horizontalPosition, segments[centipede->length-1-i].verticalPosition);
                    segments[centipede->length-1-i].xDir *= -1;

                    newSegments1.push_back(segments[centipede->length-1-i]);
                }



                newCentipede1->Init(newSegments1);
                centipedes.push_front(newCentipede1);

                Centipede * newCentipede2 = centipede_pool.FirstAvailable();
                std::vector<Centipede_segment> newSegments2;

                for(int i = 0; i < segments.size(); i++)
                    SDL_Log("Segment %i xPos = %f", i, segments[i].horizontalPosition);


                newSegments2.reserve(index);

                for(int i = 0; i < index; i++)
                {
                    SDL_Log("cent2 segment %i xPos = %f, yPos = %f", i, segments[centipede->length-1].horizontalPosition, segments[0].verticalPosition);
                    newSegments2.push_back(segments[i]);

                }



                newCentipede2->Init(newSegments2);
                centipedes.push_front(newCentipede2);
            }

        }


    }

    void InitSpider()
    {
        Spider * spider = spider_pool.FirstAvailable();
        spider->Init();
        spider_cd = system->getElapsedTime() + rand() % 20 + 10;
    }

    void InitMushrooms(int spawnPercentage)
    {

        for(int i = 2*SPRITE_HEIGHT; i < SCREEN_HEIGHT-3*SPRITE_HEIGHT; i += 33)
        {
            for(int j = 48; j < SCREEN_WIDTH-48; j += 32)
            {

                if(IsPositionEmpty(j-32, i-33)
                && IsPositionEmpty(j+32, i-33)
                && IsPositionEmpty(j-32, j+33)
                && IsPositionEmpty(j+32, j+33)
                && IsPositionEmpty(j, i))
                {
                    //srand(time(NULL));
                    int randInt = rand() % 100;

                    if(randInt < spawnPercentage)
                    {
                        Mushroom * mush = mushroom_pool.FirstAvailable();
                        mush->Init(j, i);
                        //j += 32;
                    }
                }


            }
        }

    }

    void NewGame(int extraHeads)
    {

        SDL_Log("New Game");

        ClearCentipedes();

        gameIteration++;



        player->horizontalPosition = PLAYER_INIT_X;
        player->verticalPosition = PLAYER_INIT_Y;

        InitMushrooms(20);

        Centipede * centipede = centipede_pool.FirstAvailable();
        centipede->Init(10, SCREEN_WIDTH/2, 0, 1, 1, 200);
        centipedes.push_front(centipede);

        for(int i = 0; i < extraHeads; i++){
            Centipede * cent = centipede_pool.FirstAvailable();
            cent->Init(1, rand() % SCREEN_WIDTH-64 + 32, 32, 1, 1, 750);
            centipedes.push_front(cent);
        }

    }

    void ClearCentipedes()
    {
        for(int i = 0; i < centipedes.size(); i++)
        {
            centipedes[i]->Disable();
        }

        centipedes.clear();
    }

    void IncreaseScore(int value)
    {
        score += value;
    }

    bool IsPositionEmpty(double xPos, double yPos)
    {
        for(auto mush = mushrooms.begin(); mush != mushrooms.end(); mush++)
        {
            if(abs((*mush)->horizontalPosition - xPos) < 10 && abs((*mush)->verticalPosition - yPos) < 10)
                return false;
        }
        return true;
    }

    bool IsGameOver()
    {
        return game_over;
    }

    unsigned int Score()
    {
        return score;
    }

    virtual void Destroy()
    {
        SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		life_sprite->destroy();

		delete player;
		delete rocket;
		head_pool.Destroy();
		body_pool.Destroy();
		centipede_pool.Destroy();
    }

};








