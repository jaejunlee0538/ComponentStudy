#pragma once
#include "Message.h"
class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Update() = 0;
	virtual void HandleMessage(const Message& msg){}

	void SetOwner(GameObject* owner) { m_pOwner = owner; }
protected:
	GameObject* m_pOwner;
};

