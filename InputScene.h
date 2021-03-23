#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Text.h"

class InputScene :
	public Scene
{
public:
	InputScene();
	~InputScene();

	void update(float dt);

	Sprite* bg;

	Text* name;
	Sprite* score[5];
	Sprite* best;
};

