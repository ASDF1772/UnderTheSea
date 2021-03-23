#pragma once
#include "Scene.h"
class HowToScene :
	public Scene
{
public:
	HowToScene();
	~HowToScene();

	void update(float dt);

	Sprite* bg;
	Sprite* button[3];

	int index;
};

