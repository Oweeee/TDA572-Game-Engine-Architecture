
class Game : public GameObject
{

    std::set<GameObject*> game_objects;

    std::deque<Centipede*> centipedes;

    AvancezLib* system;

    Player * player;

    Rocket * rocket;

    ObjectPool<Centipede_segment> head_pool;

    ObjectPool<Centipede_segment> body_pool;

    ObjectPool<Centipede> centipede_pool;

    Sprite * life_sprite;

    bool game_over;

    unsigned int score = 0;

    unsigned int gameIteration;

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

        rocket->Create();
        rocket->AddComponent(rocket_behaviour);
        rocket->AddComponent(rocket_render);
        rocket->AddComponent(head_collider);
        rocket->AddComponent(body_collider);
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

        player->Create();
        player->AddComponent(player_behaviour);
        player->AddComponent(player_render);
        player->AddComponent(player_headCollider);
        player->AddComponent(player_bodyCollider);
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


            (*head)->Create();
            (*head)->AddComponent(head_behaviour);
            (*head)->AddComponent(head_render);
            (*head)->AddComponent(head_collider);
            (*head)->AddReceiver(this);
        }

        body_pool.Create(20);
        for(auto body = body_pool.pool.begin(); body != body_pool.pool.end(); body++)
        {
            SegmentBehaviourComponent * body_behaviour = new SegmentBehaviourComponent();
            body_behaviour->Create(system, *body, &game_objects);
            RenderComponent * body_render = new RenderComponent();
            body_render->Create(system, *body, &game_objects, "data/body/centipede_body_right_1.bmp");

            (*body)->Create();
            (*body)->AddComponent(body_behaviour);
            (*body)->AddComponent(body_render);
            (*body)->AddReceiver(this);
        }

        centipede_pool.Create(20);

        for(auto centipede = centipede_pool.pool.begin(); centipede != centipede_pool.pool.end(); centipede++)
        {
            CentipedeBehaviourComponent * centipede_behaviour = new CentipedeBehaviourComponent();
            centipede_behaviour->Create(system, *centipede, &game_objects, &head_pool, &body_pool);



            (*centipede)->Create();
            (*centipede)->AddComponent(centipede_behaviour);
            (*centipede)->AddReceiver(this);
            game_objects.insert(*centipede);

        }
    }

    virtual void Init()
    {

        gameIteration = 1;

        player->Init();
        Centipede * centipede = centipede_pool.FirstAvailable();
        centipede->Init(10, SCREEN_WIDTH/2, 96, 1, 1, 200);
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
            rocket->enabled = false;

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
                        SplitCentipede((*it), i, centIndex);
                        break;
                    }
                }
                if(centFound)
                    break;

                centIndex++;
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

    void SplitCentipede(Centipede * centipede, int index, int centIndex)
    {

        Centipede temp = *centipede;
        Centipede_segment tail = *centipede->segments[centipede->length-1];
        Centipede_segment head = *centipede->segments[0];

        centipede->Disable();
        //centipede->Destroy();

        SDL_Log("SplitCentipede:: length = %i, index = %i", temp.length, index);

        centipedes.erase(centipedes.begin() + centIndex);
        Centipede * newCentipede1 = centipede_pool.FirstAvailable();

        if(index == temp.length-1)
        {
            if(temp.length > 1)
            {
                newCentipede1->Init(temp.length-1, temp.horizontalPosition, temp.verticalPosition, temp.xDir, temp.yDir, temp.speed);
                SDL_Log("newCentipede1 length = %i", newCentipede1->length);
                centipedes.push_front(newCentipede1);
            }

        }
        else
        {

            if(index == 0)
            {


                newCentipede1->Init(temp.length-1, tail.horizontalPosition, tail.verticalPosition, -tail.xDir, tail.yDir, tail.speed);
                SDL_Log("newCentipede1 length = %i", newCentipede1->length);
                centipedes.push_front(newCentipede1);
            }
            else
            {

                newCentipede1->Init(temp.length-(index+1), tail.horizontalPosition, tail.verticalPosition, -tail.xDir, tail.yDir, tail.speed);
                SDL_Log("newCentipede1 length = %i", newCentipede1->length);
                centipedes.push_front(newCentipede1);

                Centipede * newCentipede2 = centipede_pool.FirstAvailable();

                newCentipede2->Init(index, head.horizontalPosition, head.verticalPosition, head.xDir, head.yDir, head.speed);
                SDL_Log("newCentipede2 length = %i", newCentipede2->length);
                centipedes.push_front(newCentipede2);
            }

        }


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








