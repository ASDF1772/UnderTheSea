#pragma once
#include "Entity.h"
#include "Sprite.h"

class Model :
	public Entity
{
public:
	Model(UnitType type, TeamColor teamColor, int size, int index);
	~Model();

	void update(float dt);
	void setCell();
	bool contain(Vec2 pos);
	Vec2 offsetPos();
	Vec2 offsetCenter();

	Cell* cell;
	UnitType type;
	Vec2 offset;
	Sprite* spr;
	int size;
	Vec2 movePos;
	Vec2 originPos;

	bool is_deployed;
	bool is_auto_set;
	bool is_go_origin;

	float originRotation;
};

