#pragma once
#include "Define.h"
#include "Scene.h"
#include "Timer.h"

class World
{
public:
	World();
	~World();

	void update(float dt);
	void render();
	void init();
	void dispose();
	void changeScene(Scene* scene);
	int getKeyState(int vk);
	Vec2 getMousePos();

	float slowTime;
	Scene* currentScene;
	LPD3DXSPRITE rootSprite;
	POINT mousePoint;
	list<Timer*>timers;
	bool lastKeys[256];
	bool currentKeys[256];
};

extern World world;