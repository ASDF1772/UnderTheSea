#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Tile.h"
#include "Ship.h"
#include "StageMap.h"

class Bullet :
	public Entity
{
public:
	Bullet(BulletType type, Team team, TeamColor teamColor);
	~Bullet();

	void update(float dt);
	void explode(Tile* targetTile, Vec2 targetPos);

	BulletType type;
	Team team;
	TeamColor teamColor;
	Ship* owner;
	StageMap* targetMap;
	StageMap* ownerMap;

	Animation* ani;
	float angleWithTarget;
	float speed;
	Tile* targetTile;
	Vec2 targetPos;
	bool is_main;
	bool is_miss;
	bool away;
	bool is_increase;

	Vec2 missPos;
	float missTimer;
	float bubbleTimer;
};