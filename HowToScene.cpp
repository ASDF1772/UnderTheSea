#include "DXUT.h"
#include "HowToScene.h"
#include "World.h"
#include "MainScene.h"
#include "Asset.h"


HowToScene::HowToScene() :index(0)
{
	bg = new Sprite("image/HowTo/0.png");
	addChild(bg);

	for (int i = 0; i < 3; i++)
	{
		button[i] = new Sprite("image/HowTo/Button/" + to_string(i) + ".png");
		addChild(button[i]);
		button[i]->pos = Vec2(28 + i * 1115, 363);
		button[i]->scaleCenter = button[i]->rect.center();
	}
	button[2]->pos = Vec2(1106, 35);
}


HowToScene::~HowToScene()
{
}


void HowToScene::update(float dt)
{
	Scene::update(dt);
	if (!asset.sounds[L"image/Sound/main.wav"]->IsSoundPlaying())
	{
		asset.sounds[L"image/Sound/main.wav"]->Play();
	}

	for (int i = 0; i < 3; i++)
	{
		if (button[i]->rectWithPos().contain(world.getMousePos()))
		{
			button[i]->scale = Vec2(1.1, 1.1);

			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				if (i == 0)
				{
					if(index > 0)
						index--;
				}
				else if (i == 1)
				{
					if (index < 1)
						index++;
				}
				else if (i == 2)
				{
					world.changeScene(new MainScene());
					return;
				}
				bg->setPath("image/HowTo/" + to_string(index) + ".png");
			}
		}
		else
		{
			button[i]->scale = Vec2(1, 1);
		}
	}

	if (index == 0)
	{
		button[0]->visible = false;
		button[1]->visible = true;
	}
	else if (index == 1)
	{
		button[0]->visible = true;
		button[1]->visible = false;
	}

}