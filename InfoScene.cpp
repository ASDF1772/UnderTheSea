#include "DXUT.h"
#include "InfoScene.h"
#include "World.h"
#include "MainScene.h"
#include "Asset.h"


InfoScene::InfoScene()
{
	bg = new Sprite("image/Info/bg.png");
	close = new Sprite("image/Info/close.png");

	addChild(bg);
	addChild(close);

	close->pos = Vec2(1106, 35);

	close->scaleCenter = close->rect.center();
}


InfoScene::~InfoScene()
{
}


void InfoScene::update(float dt)
{
	Scene::update(dt);

	if (!asset.sounds[L"image/Sound/main.wav"]->IsSoundPlaying())
	{
		asset.sounds[L"image/Sound/main.wav"]->Play();
	}

	if (close->rectWithPos().contain(world.getMousePos()))
	{
		close->scale = Vec2(1.1, 1.1);

		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			world.changeScene(new MainScene());
			return;
		}
	}
	else
	{
		close->scale = Vec2(1, 1);
	}
}