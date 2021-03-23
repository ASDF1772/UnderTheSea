#pragma once
#include "Scene.h"

class SelectScene :
	public Scene
{
public:
	SelectScene();
	~SelectScene();

	void update(float dt);

	Sprite* bg;
	Sprite* basic[2];
	Sprite* focus[2];
	Sprite* info[2];
};

