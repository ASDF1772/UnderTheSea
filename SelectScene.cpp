#include "DXUT.h"
#include "SelectScene.h"
#include "World.h"
#include "DeployScene.h"
#include "GameManager.h"
#include "Asset.h"


SelectScene::SelectScene()
{
	bg = new Sprite("image/Select/bg.png");

	addChild(bg);

	for (int i = 0; i < 2; i++)
	{
		basic[i] = new Sprite("image/Select/Basic/" + to_string(i) + ".png");
		focus[i] = new Sprite("image/Select/Focus/" + to_string(i) + ".png");
		info[i] = new Sprite("image/Select/Info/" + to_string(i) + ".png");

		addChild(basic[i]);
		addChild(focus[i]);
		addChild(info[i]);

		focus[i]->pos = basic[i]->pos = Vec2(189 + i * 472, 233 - i * 19);
		info[i]->visible = false;
	}
}


SelectScene::~SelectScene()
{
}


void SelectScene::update(float dt)
{
	Scene::update(dt);
	if (!asset.sounds[L"image/Sound/main.wav"]->IsSoundPlaying())
	{
		asset.sounds[L"image/Sound/main.wav"]->Play();
	}

	for (int i = 0; i < 2; i++)
	{
		if (basic[i]->rectWithPos().contain(world.getMousePos()))
		{
			for (int j = 0; j < 2; j++)
			{
				basic[j]->visible = false;
			}
			basic[i]->visible = false;
			focus[i]->visible = true;
			info[i]->visible = true;

			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				if (i == 0)
				{
					gm.allyColor = BLUE;
					gm.enemyColor = RED;
				}
				else 
				{
					gm.allyColor = RED;
					gm.enemyColor = BLUE;
				}
				world.changeScene(new DeployScene());
				return;
			}
			break;
		}
		else
		{
			basic[i]->visible = true;
			focus[i]->visible = false;
			info[i]->visible = false;
		}
	}
}