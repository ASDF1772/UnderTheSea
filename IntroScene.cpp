#include "DXUT.h"
#include "IntroScene.h"
#include "World.h"
#include "MainScene.h"
#include "Asset.h"


IntroScene::IntroScene()
{
	ani = new Animation("image/Intro/", 40, 10, 0, 0);

	addChild(ani);

	updateEnabled = false;
}


IntroScene::~IntroScene()
{
}


void IntroScene::update(float dt)
{
	Scene::update(dt);

	if (!asset.sounds[L"image/Sound/main.wav"]->IsSoundPlaying())
	{
		asset.sounds[L"image/Sound/main.wav"]->Play();
	}

	if (world.getKeyState(VK_LBUTTON) == 1)
	{
		updateEnabled = true;
	}

	if (ani->is_end)
	{
		world.changeScene(new MainScene());
		return;
	}
}