#include "DXUT.h"
#include "DeployUI.h"
#include "World.h"
#include "GameManager.h"


DeployUI::DeployUI()
{
	for (int i = 0; i < 3; i++)
	{
		button[i] = new Sprite("image/Deploy/UI/Button/" + to_string(i) + ".png");
		addChild(button[i]);
		button[i]->scaleCenter = button[i]->rect.center();
		button[i]->pos = Vec2(461 + i * 255, 599);
	}
	button[2]->pos = Vec2(1019, 638);

	for (int i = 0; i < 2; i++)
	{
		item[i] = new Sprite("image/Deploy/UI/Item/" + to_string(i) + ".png");
		addChild(item[i]);
		item[i]->scaleCenter = item[i]->rect.center();
		item[i]->pos = Vec2(1002 + i * 126, 513);
	}
}


DeployUI::~DeployUI()
{
}


void DeployUI::update(float dt)
{
	Entity::update(dt);

	for (int i = 0; i < 3; i++)
	{
		if (button[i]->visible && button[i]->rectWithPos().contain(world.getMousePos()))
		{
			button[i]->scale = Vec2(1.1, 1.1);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				if (i == 0)
				{
					gm.indeploy->randomDeploy();
				}
				else if (i == 1)
				{
					gm.indeploy->resetDeploy();
				}
				else
				{
					gm.indeploy->changeScene();
				}
			}
		}
		else
		{
			button[i]->scale = Vec2(1, 1);
		}
	}

	bool is_all_deployed = true;
	for (int i = 0; i < 5; i++)
	{
		if (!gm.indeploy->deployMap->ship[i]->is_deployed)
			is_all_deployed = false;
	}
	if (is_all_deployed)
	{
		button[2]->visible = true;
	}
	else
	{
		button[2]->visible = false;
	}

	for (int i = 0; i < 2; i++)
	{
		if (gm.haveDeployItem[i])
		{
			if (item[i]->rectWithPos().contain(world.getMousePos()))
			{
				item[i]->scale = Vec2(1.1, 1.1);

				if (world.getKeyState(VK_LBUTTON) == -1)
				{
					if (i == 0)
					{
						gm.indeploy->cursor->setPath("image/Game/UI/cursor/mine.png");
					}
					else if (i == 1)
					{
						gm.indeploy->cursor->setPath("image/Game/UI/cursor/shovel.png");
					}
					gm.useDeployItem[i] = true;
				}
			}
			else
			{
				item[i]->scale = Vec2(1, 1);
			}
		}
	}
}