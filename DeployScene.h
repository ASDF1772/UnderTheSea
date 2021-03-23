#pragma once
#include "Scene.h"
#include "DeployMap.h"
#include "DeployUI.h"

class DeployScene :
	public Scene
{
public:
	DeployScene();
	~DeployScene();

	void render();
	void update(float dt);
	void refresh();
	void resetDeploy();
	void randomDeploy();
	void changeScene();

	Sprite* bg;
	Sprite* frame;
	DeployUI* ui;
	DeployMap* deployMap;
	DeployMap* enemyMap;
	Model* target;
	bool is_change_scene;
};