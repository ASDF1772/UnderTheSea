#pragma once
#include "Scene.h"
class InfoScene :
	public Scene
{
public:
	InfoScene();
	~InfoScene();

	void update(float dt);

	Sprite* bg;
	Sprite* close;
};