#pragma once
#include "Scene.h"
#include "Animation.h"
#include "Particle.h"

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();

	void render();
	void update(float dt);

	Animation* bg;
	Sprite* logo;
	Sprite* button[5];
	vector<Particle*> particleList;

	Vec2 lastMousePos;
	float mouseSpeed;
	float buttonTimer;
	float bubbleTimer;
	Timer spawnTimer;
	float bubblePos;
};

