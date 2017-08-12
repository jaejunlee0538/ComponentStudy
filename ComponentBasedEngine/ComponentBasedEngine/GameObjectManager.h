#pragma once
class GameObject;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	GameObject* GetGameObject(std::string name);
};

