#pragma once
#include "Entity.h"
#include "Sprite.h"

class DeployTile :
	public Entity
{
public:
	DeployTile();
	~DeployTile();

	Sprite* spr;
	Sprite* filter;
};