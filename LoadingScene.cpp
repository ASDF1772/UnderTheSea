#include "DXUT.h"
#include "LoadingScene.h"
#include "World.h"
#include "Asset.h"
#include "IntroScene.h"


LoadingScene::LoadingScene()
{
	bg = new Animation("image/Loading/ani/", 21, 10, 1);

	addChild(bg);

	frame = new Sprite("image/Loading/0.png");
	bar = new Sprite("image/Loading/1.png");

	addChild(frame);
	addChild(bar);

	bar->visibleRect.right = 0;

	bar->pos = Vec2(100, 630);

	asset.ready("image/");
	asset.getSound(L"image/Sound/main.wav");
	asset.getSound(L"image/Sound/ingame.wav");
	asset.getSound(L"image/Sound/explode.wav");
	asset.getSound(L"image/Sound/fire.wav");
	asset.getSound(L"image/Sound/win.wav");
	asset.getSound(L"image/Sound/lose.wav");
}


LoadingScene::~LoadingScene()
{
}


void LoadingScene::update(float dt)
{
	Scene::update(dt);

	for (int i = 0; i < 5; i++)
	{
		if (asset.fileToLoad == asset.fileLoaded)
		{
			world.changeScene(new IntroScene());
			return;
		}
		asset.loadNext();
	}

	bar->visibleRect.right = (float)asset.fileLoaded / (float)asset.fileToLoad * bar->rect.right;
}