#pragma once
#include "Entity.h"
#include "Sprite.h"
#include "Ship.h"

class Tile :
	public Entity
{
public:
	Tile(TileType type, TeamColor teamColor);
	~Tile();

	void update(float dt);
	void setLimit();
	Vec2 center();

	Cell index;
	TileType type;
	Sprite* spr;
	Sprite* filter;
	Sprite* mark;
	Sprite* debris;
	Ship* ship;

	bool is_limit;
	bool is_smoke;

	float moveAngle;
	float smokeTimer;
};