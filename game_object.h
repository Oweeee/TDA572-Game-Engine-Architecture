// GameObject represents objects which moves are drawn
#include <vector>

enum Message { ROCKET, SPLIT_CENTIPEDE, CENTIPEDE, PLAYER, PLAYER_HIT, GAME_OVER, LEVEL_WIN, NO_MSG };

class Component;

class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;

public:
	double horizontalPosition;
	double verticalPosition;
	bool enabled;
	Message m;
    char* name;

	virtual ~GameObject();

	virtual void Create();
	virtual void AddComponent(Component * component);

	virtual void Init(char* name, Message m);
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	virtual bool IsEnabled();
	void Send(Message m);

};
