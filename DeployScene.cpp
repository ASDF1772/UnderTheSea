#include "DXUT.h"
#include "DeployScene.h"
#include "World.h"
#include "GameManager.h"
#include "Asset.h"


DeployScene::DeployScene() :target(nullptr), is_change_scene(false)
{
	gm.indeploy = this;
	gm.init();

	bg = new Sprite("image/Deploy/bg.png");
	addChild(bg);

	frame = new Sprite("image/Deploy/UI/frame.png");
	addChild(frame);

	ui = new DeployUI();

	deployMap = new DeployMap(gm.allyColor);
	enemyMap = new DeployMap(gm.enemyColor);

	addChild(deployMap);

	deployMap->mapInfo = gm.pMap;
	enemyMap->mapInfo = gm.eMap;

	for (int i = 0; i < 2; i++)
	{
		deployMap->objectInfo[i] = gm.pObject[i];
		enemyMap->objectInfo[i] = gm.eObject[i];
	}

	deployMap->init();
	enemyMap->init();

	deployMap->pos = Vec2(419, 30);

	asset.sounds[L"image/Sound/main.wav"]->Stop();
	asset.sounds[L"image/Sound/ingame.wav"]->Reset();
}


DeployScene::~DeployScene()
{
	delete ui;
	delete enemyMap;
}


void DeployScene::render()
{
	Scene::render();

	ui->render();

	if (target) target->render();
}


void DeployScene::update(float dt)
{
	Scene::update(dt);

	if (!asset.sounds[L"image/Sound/ingame.wav"]->IsSoundPlaying())
	{
		asset.sounds[L"image/Sound/ingame.wav"]->Play();
	}

	ui->update(dt);

	if (is_change_scene)
	{
		world.changeScene(new GameScene());
		return;
	}

	if (world.getKeyState(VK_LBUTTON) == 1)
	{
		if (gm.useDeployItem[0])
		{
			int x = (world.getMousePos().x - deployMap->pos.x) / DEPLOY_SIZE;
			int y = (world.getMousePos().y - deployMap->pos.y) / DEPLOY_SIZE;
			Model* mine = new Model(MINE, gm.allyColor, 1, 1);
			if (deployMap->checkDeploy(mine, x, y))
			{
				mine->is_deployed = true;
				mine->pos = Vec2(x, y) * DEPLOY_SIZE;
				deployMap->addChild(mine);
				deployMap->mine = mine;
				deployMap->mapInfo->haveMine = true;
				deployMap->mapInfo->type[y][x] = MINE_TILE;
				deployMap->tile[y][x]->filter->visible = true;
				gm.haveDeployItem[0] = false;
				ui->item[0]->color.a = 0.3;
			}
			else
			{
				delete mine;
			}
			gm.useDeployItem[0] = false;
			gm.indeploy->cursor->setPath("image/Game/UI/cursor/basic.png");
		}
		else if (gm.useDeployItem[1])
		{
			for (int i = 0; i < 2; i++)
			{
				if (deployMap->mapInfo->haveObject[i] && deployMap->object[i]->contain(world.getMousePos()))
				{
					deployMap->mapInfo->haveObject[i] = false;
					deployMap->setTile(deployMap->object[i], BASIC_TILE);
					refresh();
					deployMap->removeChild(deployMap->object[i]);
					deployMap->object[i] = nullptr;
					gm.haveDeployItem[1] = false;
					ui->item[1]->color.a = 0.3;
				}
			}
			gm.indeploy->cursor->setPath("image/Game/UI/cursor/basic.png");
			gm.useDeployItem[1] = false;
		}
		else
		{
			for (int i = 0; i < 5; i++)
			{
				if (deployMap->ship[i]->contain(world.getMousePos()))
				{
					target = deployMap->ship[i];

					if (target->is_deployed)
					{
						target->is_deployed = false;
						deployMap->setTile(target, BASIC_TILE);
						refresh();
					}
				}
			}
			if (deployMap->mapInfo->haveMine)
			{
				if (deployMap->mine->contain(world.getMousePos()))
				{
					target = deployMap->mine;

					if (target->is_deployed)
					{
						target->is_deployed = false;
						int x = target->offsetCenter().x / DEPLOY_SIZE;
						int y = target->offsetCenter().y / DEPLOY_SIZE;
						deployMap->mapInfo->type[y][x] = BASIC_TILE;
						deployMap->tile[y][x]->filter->visible = false;
						refresh();
					}
				}
			}
		}
	}

	if (target)
	{
		target->pos += (world.getMousePos() - deployMap->pos - target->offsetCenter()) * 0.15;

		if (world.getKeyState(VK_LBUTTON) == -1)
		{
			int x = target->offsetCenter().x / DEPLOY_SIZE;
			int y = target->offsetCenter().y / DEPLOY_SIZE;

			if (deployMap->checkDeploy(target, x, y))
			{
				target->is_deployed = true;
				if (target->type == MINE)
				{
					deployMap->mapInfo->type[y][x] = MINE_TILE;
					deployMap->tile[y][x]->filter->visible = true;
				}
				else
				{
					deployMap->setTile(target, UNIT_TILE);
				}
				target->pos = Vec2(x, y) * DEPLOY_SIZE - target->offset;
			}
			else
			{
				if (target->type == MINE)
				{
					while (1)
					{
						int x = random(0, 9);
						int y = random(0, 9);

						if (deployMap->checkDeploy(target, x, y))
						{
							target->movePos = Vec2(x, y) * DEPLOY_SIZE;
							target->is_auto_set = true;
							deployMap->mapInfo->type[y][x] = MINE_TILE;
							deployMap->tile[y][x]->filter->visible = true;
							break;
						}
					}
				}
				else
				{
					target->is_go_origin = true;
					target->rotation = target->originRotation;
					target->setCell();
				}
			}
			target = nullptr;
		}
		else if (world.getKeyState(VK_RBUTTON) == 1)
		{
			target->rotation += D3DXToRadian(90);

			if (target->rotation == D3DXToRadian(360))
				target->rotation = 0;

			target->setCell();
		}
	}
}


void DeployScene::refresh()
{
	for (int i = 0; i < 5; i++)
	{
		if (deployMap->ship[i]->is_deployed)
		{
			deployMap->setTile(deployMap->ship[i], UNIT_TILE);
		}
	}
	for (int i = 0; i < 2; i++)
	{
		if (deployMap->mapInfo->haveObject[i])
		{
			deployMap->setTile(deployMap->object[i], OBJECT_TILE);
		}
	}
	if (deployMap->mapInfo->haveMine && deployMap->mine->is_deployed)
	{
		int x = deployMap->mine->offsetCenter().x / DEPLOY_SIZE;
		int y = deployMap->mine->offsetCenter().y / DEPLOY_SIZE;

		deployMap->tile[y][x]->filter->visible = true;
	}
}


void DeployScene::resetDeploy()
{
	bool can_deploy = true;

	for (int i = 0; i < 5; i++)
	{
		if (deployMap->ship[i]->is_go_origin || deployMap->ship[i]->is_auto_set)
			can_deploy = false;
	}

	if (can_deploy)
	{
		for (int i = 0; i < 5; i++)
		{
			if (deployMap->ship[i]->is_deployed)
			{
				deployMap->setTile(deployMap->ship[i], BASIC_TILE);
				deployMap->ship[i]->rotation = deployMap->ship[i]->originRotation;
				deployMap->ship[i]->setCell();
				deployMap->ship[i]->is_go_origin = true;
				deployMap->ship[i]->is_deployed = false;
			}
		}
		refresh();
	}
}


void DeployScene::randomDeploy()
{
	bool can_deploy = true;

	for (int i = 0; i < 5; i++)
	{
		if (deployMap->ship[i]->is_go_origin || deployMap->ship[i]->is_auto_set)
			can_deploy = false;
	}

	if (can_deploy)
	{
		for (int i = 0; i < 5; i++)
		{
			if (deployMap->ship[i]->is_deployed)
			{
				deployMap->setTile(deployMap->ship[i], BASIC_TILE);
				deployMap->ship[i]->rotation = deployMap->ship[i]->originRotation;
				deployMap->ship[i]->setCell();
				deployMap->ship[i]->is_deployed = false;
			}
		}
		refresh();

		for (int i = 0; i < 5; i++)
		{
			int x = random(0, 9);
			int y = random(0, 9);
			int dir = random(0, 3);
			deployMap->ship[i]->rotation = dir * D3DXToRadian(90);
			deployMap->ship[i]->setCell();

			if (deployMap->checkDeploy(deployMap->ship[i], x, y))
			{
				deployMap->ship[i]->is_auto_set = true;
				deployMap->ship[i]->movePos = Vec2(x, y) * DEPLOY_SIZE - deployMap->ship[i]->offset;
				deployMap->setTile(deployMap->ship[i], UNIT_TILE, x, y);
			}
			else
			{
				i--;
				continue;
			}
		}
	}
}


void DeployScene::changeScene()
{
	if (deployMap->mapInfo->haveMine)
	{
		gm.mine->dir = 0;
		gm.mine->pos.x = deployMap->mine->offsetCenter().x / DEPLOY_SIZE;
		gm.mine->pos.y = deployMap->mine->offsetCenter().y / DEPLOY_SIZE;
		gm.mine->size = 1;
	}

	for (int i = 0; i < 5; i++)
	{
		gm.pShip[i]->dir = deployMap->ship[i]->rotation / D3DXToRadian(90);
		gm.pShip[i]->pos.x = deployMap->ship[i]->offsetCenter().x / DEPLOY_SIZE;
		gm.pShip[i]->pos.y = deployMap->ship[i]->offsetCenter().y / DEPLOY_SIZE;
		gm.pShip[i]->size = i / 2 + 2;
	}

	for (int i = 0; i < 5; i++)
	{
		int x = random(0, 9);
		int y = random(0, 9);
		int dir = random(0, 3);
		enemyMap->ship[i]->rotation = dir * D3DXToRadian(90);
		enemyMap->ship[i]->setCell();

		if (enemyMap->checkDeploy(enemyMap->ship[i], x, y))
		{
			enemyMap->ship[i]->pos = Vec2(x, y) * DEPLOY_SIZE - enemyMap->ship[i]->offset;
			enemyMap->setTile(enemyMap->ship[i], UNIT_TILE, x, y);
			gm.eShip[i]->dir = dir;
			gm.eShip[i]->pos = Cell(x, y);
			gm.eShip[i]->size = i / 2 + 2;
		}
		else
		{
			i--;
			continue;
		}
	}

	is_change_scene = true;
}