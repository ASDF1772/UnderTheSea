#pragma once
#include "Define.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void update(float dt);
	virtual void render();
	void addChild(Entity* child);
	void removeChild(Entity* child);
	void setCenter(Vec2 pos);
	Vec2 center();
	Rect rectWithPos();

	D3DXMATRIX matrix;
	list<Entity*>children;
	Entity* parent;
	Rect rect;
	Rect visibleRect;
	Vec2 pos;
	Vec2 scale;
	Vec2 scaleCenter;
	float rotation;
	Vec2 rotationCenter;
	bool updateEnabled;
	bool renderChildrenEnabled;
	bool visible;
	bool removing;
	bool deleting;
};