#pragma once
#include "Entity.h"
#include "Animation.h"

class Ship :
	public Entity
{
public:
	Ship(UnitType type, Team team, TeamColor teamColor, int dir, int size, int index);
	~Ship();

	void update(float dt);

	Sprite* spr;
	UnitType type;
	Team team;
	TeamColor teamColor;

	int dir;
	int size;
	int index;
	Cell mapPos;
	Cell* cell;
	bool is_die;
	bool die_end;
	int hitCount;
};