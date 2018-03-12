#include "component.h"
#include "game_object.h"
#include "avancezlib.h"

void Component::Create(AvancezLib * system, GameObject * go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->system = system;
	this->game_objects = game_objects;
}

void RenderComponent::Create(AvancezLib * system, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name)
{
	Component::Create(system, go, game_objects);

	sprite = system->createSprite(sprite_name);
}

void RenderComponent::Update(float dt)
{
	if (!go->enabled)
		return;

	if (sprite)
		sprite->draw(int(go->horizontalPosition), int(go->verticalPosition));
}

void RenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
	sprite = NULL;
}


void CollideComponent::Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects)
{
	Component::Create(system, go, game_objects);
	this->coll_objects = coll_objects;
}


void CollideComponent::Update(float dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if(go != go0)
        {
            if (go0->enabled)
            {
                float go_xMin = (go->horizontalPosition + 16.f) - (go->hitBox_x/2);
                float go_xMax = (go->horizontalPosition + 16.f) + (go->hitBox_x/2);
                float go_yMin = (go->verticalPosition + 16.f) - (go->hitBox_y/2);
                float go_yMax = (go->verticalPosition + 16.f) + (go->hitBox_y/2);

                float go0_xMin = (go0->horizontalPosition + 16.f) - (go0->hitBox_x/2);
                float go0_xMax = (go0->horizontalPosition + 16.f) + (go0->hitBox_x/2);
                float go0_yMin = (go0->verticalPosition + 16.f) - (go0->hitBox_y/2);
                float go0_yMax = (go0->verticalPosition + 16.f) + (go0->hitBox_y/2);

                if(!(go0_xMin > go_xMax || go0_xMax < go_xMin || go0_yMin > go_yMax || go0_yMax < go_yMin))
                {

                    if(go0->m == go->m)
                    {
                        go->Receive(go->m);
                    }

                    else
                    {

                        go->Receive(go0->m);
                        go0->Receive(go->m);
                    }






			}
		}
        }

	}
}
