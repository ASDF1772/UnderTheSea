#pragma once
#include "Entity.h"
#include "Sprite.h"

class DeployUI :
	public Entity
{
public:
	DeployUI();
	~DeployUI();

	void update(float dt);

	Sprite* button[3];
	Sprite* item[2];
};