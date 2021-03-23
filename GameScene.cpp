#include "DXUT.h"
#include "GameScene.h"
#include "GameManager.h"
#include "World.h"
#include "DeployScene.h"
#include "EndingScene.h"
#include "Asset.h"


GameScene::GameScene() 
	:pFocusTile(nullptr), eFocusTile(nullptr), is_player_turn(true), is_enemy_turn(false), is_player_attack(true), is_enemy_attack(false),
	bubbleTimer(0), moveAngle(0), sceneTimer(0), turnTimer(0), is_player_turn_end(false), is_enemy_turn_end(false), fishTimer(0)
{
	gm.ingame = this;

	ui = new UI();

	bg = new Sprite("image/Game/bg.png");
	addChild(bg);
	bg->pos = Vec2(-100, -100);

	playerMap = new StageMap(ALLY, gm.allyColor);
	enemyMap = new StageMap(ENEMY, gm.enemyColor);

	addChild(playerMap);
	addChild(enemyMap);

	playerMap->mapInfo = gm.pMap;
	enemyMap->mapInfo = gm.eMap;

	for (int i = 0; i < 5; i++)
	{
		playerMap->shipInfo[i] = gm.pShip[i];
		enemyMap->shipInfo[i] = gm.eShip[i];
	}

	for (int i = 0; i < 2; i++)
	{
		playerMap->objectInfo[i] = gm.pObject[i];
		enemyMap->objectInfo[i] = gm.eObject[i];
	}

	playerMap->mineInfo = gm.mine;

	playerMap->init();
	enemyMap->init();

	playerMap->pos = Vec2(249, 295);
	enemyMap->pos = Vec2(568, 72);


	pointer = new Sprite("image/Game/UI/pointer.png");
	addChild(pointer);
	pointer->setCenter(playerMap->pos + playerMap->ship[0]->center() - Vec2(0, 70));

	thinkTimer.first = 0;
	thinkTimer.second = 0;
	lastMousePos = Vec2(0, 0);
}


GameScene::~GameScene()
{
	delete(ui);
}


void GameScene::render()
{
	Scene::render();

	ui->render();
}


void GameScene::update(float dt)
{
	Scene::update(dt);
	if (!asset.sounds[L"image/Sound/ingame.wav"]->IsSoundPlaying())
	{
		asset.sounds[L"image/Sound/ingame.wav"]->Play();
	}

	ui->update(dt);
	bubbleTimer += dt;
	fishTimer += dt;

	for (int i = 0; i < (int)distance(lastMousePos, world.getMousePos()) / 30; i++)
	{
		Particle* p = new Particle("image/Game/Effect/bubble.png", BUBBLE2);
		addChild(p);
		particleList.push_back(p);
		p->scaleCenter = p->rect.center();
		p->scale = Vec2(1, 1) * random(0.4f, 0.8f);
		p->pos = world.getMousePos() + Vec2(random(-20, 20), random(-20, 20));
		p->color.a = 0.7;
		p->moveAngle = D3DXToRadian(270);
	}
	lastMousePos = world.getMousePos();

	if (bubbleTimer > 1)
	{
		bubblePos = random(0, 1280);
		spawnTimer.reset(0.2, 6);
		spawnTimer.onTick = [=]()
		{
			if (!random(0, 5)) return;

			Particle* p = new Particle("image/Game/Effect/bubble.png", BUBBLE);
			addChild(p);
			particleList.push_back(p);
			p->scaleCenter = p->rect.center();
			p->scale = Vec2(1, 1) * random(0.6f, 1.2f);
			p->pos = Vec2(bubblePos + random(-30, 30), 720);
			p->color.a = 0.7;
		};
		bubbleTimer = 0;
	}

	if (fishTimer > 7)
	{
		Animation* a = new Animation("image/Game/Effect/Fish/", 21, 21, true);
		addChild(a);
		fishList.push_back(a);
		a->pos = Vec2(1280, random(0, 620));
		a->scale = Vec2(0.8, 0.8);
		fishTimer = 0;
	}

	if (pointer->visible)
	{
		moveAngle += 3 * dt;
		pointer->pos.y += sin(moveAngle) * 30 * dt;
	}

	if (world.getKeyState(VK_F3) == 1)
	{
		gm.is_win = true;
	}
	else if (world.getKeyState(VK_F4) == 1)
	{
		gm.is_lose = true;
	}
	if (world.getKeyState(VK_F5) == 1)
	{
		for (int i = 0; i < 5; i++)
		{
			if (!enemyMap->ship[i]->is_die)
			{
				enemyMap->ship[i]->visible = true;
			}
		}
	}
	else if (world.getKeyState(VK_F6) == 1)
	{
		for (int i = 0; i < 5; i++)
		{
			if (!enemyMap->ship[i]->is_die)
			{
				enemyMap->ship[i]->visible = false;
			}
		}
	}
	else if (world.getKeyState(VK_F7) == 1)
	{
		gm.ai->attackMine = true;
	}
	else if (world.getKeyState(VK_F8) == 1)
	{
		gm.score += 500;
	}


	if (gm.is_win)
	{
		if (sceneTimer == 0)
		{
			Particle* p = new Particle("image/Game/UI/win.png", FADE, 1, 1, true);
			addChild(p); 
			particleList.push_back(p);
		}
		sceneTimer += dt;
		if (sceneTimer > 2)
		{
			world.changeScene(new EndingScene(gm.allyColor, true));
			return;
		}
	}
	else if (gm.is_lose)
	{
		if (sceneTimer == 0)
		{
			Particle* p = new Particle("image/Game/UI/lose.png", FADE, 1, 1, true);
			addChild(p);
			particleList.push_back(p);
		}
		sceneTimer += dt;
		if (sceneTimer > 2)
		{
			world.changeScene(new EndingScene(gm.enemyColor, false));
			return;
		}
	}
	else
	{
		if (is_player_turn)
		{
			if (is_player_attack)
			{
				Tile* mouseTile = enemyMap->getTile(world.getMousePos());
				if (mouseTile) {
					cursor->setPath("image/Game/UI/cursor/attack.png");
				}
				else {
					cursor->setPath("image/Game/UI/cursor/basic.png");
				}

				if (pFocusTile && mouseTile)
				{
					if (pFocusTile != mouseTile)
					{
						setFilter(pFocusTile, false);

						pFocusTile = mouseTile;

						setFilter(pFocusTile, true);
					}
				}
				else if (pFocusTile && !mouseTile)
				{
					setFilter(pFocusTile, false);
					pFocusTile = nullptr;
				}
				else if (!pFocusTile && mouseTile)
				{
					pFocusTile = mouseTile;
					setFilter(pFocusTile, true);
				}

				if (pFocusTile)
				{
					if (world.getKeyState(VK_LBUTTON) == 1)
					{
						spawnBullet(gm.bulletType, ALLY, pFocusTile);

						if (gm.pUseSkill && gm.allyColor == RED && gm.bulletType == BASIC_BULLET)
						{
							int i = 0;
							while (1)
							{
								int x_tmp = random(-2, 2);
								int y_tmp = random(-2, 2);

								if (y_tmp == 0 && x_tmp == 0) continue;

								int y = pFocusTile->index.y + y_tmp;
								int x = pFocusTile->index.x + x_tmp;

								if (9 < y || y < 0) continue;
								if (9 < x || x < 0) continue;

								if (!enemyMap->tile[y][x]->is_limit)
								{
									spawnBullet(BASIC_BULLET, ALLY, enemyMap->tile[y][x], false, true);
									break;
								}
								if (i++ > 10) break;
							}

						}

						if (gm.bulletType == BARRAGE_BULLET)
						{
							gm.coolDown[0] = 0;
							gm.haveItem[0] = false;
							gm.useItem[0] = false;
							ui->check_icon[0]->visible = false;
							ui->item_icon[0]->color.a = 0.3;

							pBarrageTimer.reset(0.1, 4);
							pBarrageTimer.onTick = [=]()
							{
								float dir = pBarrageTimer.count.first * D3DXToRadian(90);
								int x = pFocusTile->index.x + (int)cos(dir);
								int y = pFocusTile->index.y + (int)sin(dir);

								if (9 < y || y < 0) return;
								if (9 < x || x < 0) return;
								if (enemyMap->tile[y][x]->is_limit) return;

								spawnBullet(gm.bulletType, ALLY, enemyMap->tile[y][x], false);
							};
						}
						else if (gm.bulletType == FLARE_BULLET)
						{
							gm.coolDown[1] = 0;
							gm.useItem[1] = false;
							gm.haveItem[1] = false;
							ui->check_icon[1]->visible = false;
							ui->item_icon[1]->color.a = 0.3;
						}

						cursor->setPath("image/Game/UI/cursor/basic.png");
						is_player_attack = false;
						asset.sounds[L"image/Sound/fire.wav"]->Reset();
						asset.sounds[L"image/Sound/fire.wav"]->Play();
					}
				}
			}
		}
		if (is_enemy_turn)
		{
			if (is_enemy_attack)
			{
				if (thinkTimer.first == 0)
				{
					thinkTimer.second = random(1.f, 2.f);
				}
				thinkTimer.first += dt;

				if (thinkTimer.first > thinkTimer.second)
				{
					thinkTimer.first = 0;
					eFocusTile = gm.ai->getTargetTile();

					if (eFocusTile)
					{
						asset.sounds[L"image/Sound/fire.wav"]->Reset();
						asset.sounds[L"image/Sound/fire.wav"]->Play();
						if (gm.turn % 5 == 2)
						{
							spawnBullet(BARRAGE_BULLET, ENEMY, eFocusTile);

							eBarrageTimer.reset(0.1, 4);
							eBarrageTimer.onTick = [=]()
							{
								float dir = eBarrageTimer.count.first * D3DXToRadian(90);
								int x = eFocusTile->index.x + (int)cos(dir);
								int y = eFocusTile->index.y + (int)sin(dir);

								if (9 < y || y < 0) return;
								if (9 < x || x < 0) return;
								if (playerMap->tile[y][x]->is_limit) return;

								spawnBullet(BARRAGE_BULLET, ENEMY, playerMap->tile[y][x], false);
							};
						}
						else
						{
							spawnBullet(BASIC_BULLET, ENEMY, eFocusTile);

							if (gm.eUseSkill && gm.enemyColor == RED)
							{
								int i = 0;
								while (1)
								{
									int x_tmp = random(-2, 2);
									int y_tmp = random(-2, 2);

									if (y_tmp == 0 && x_tmp == 0) continue;

									int y = eFocusTile->index.y + y_tmp;
									int x = eFocusTile->index.x + x_tmp;

									if (9 < y || y < 0) continue;
									if (9 < x || x < 0) continue;

									if (!playerMap->tile[y][x]->is_limit)
									{
										spawnBullet(BASIC_BULLET, ENEMY, playerMap->tile[y][x], false, true);
										break;
									}
									if (i++ > 10) break;
								}
							}
						}
						is_enemy_attack = false;
					}
				}
			}
		}

		if (is_player_turn_end || is_enemy_turn_end)
		{
			turnTimer += dt;
			if (turnTimer > 0.5)
			{
				turnTimer = 0;
				if (is_player_turn_end)
				{
					if (gm.allyColor == RED && gm.pUseSkill)
					{
						gm.skillCool = 0;
						gm.haveSkill = false;
						gm.pUseSkill = false;
						ui->skill_check->visible = false;
						ui->skill_icon->color.a = 0.3;
					}
					if (gm.enemyColor == BLUE)
						gm.eUseSkill = false;
					if (gm.turn % 3 == 2) gm.eUseSkill = true;

					pFocusTile = nullptr;
					is_player_turn_end = false;
					is_enemy_turn = true;
					is_enemy_attack = true;
					gm.bulletType = BASIC_BULLET;
					pointer->visible = false;
					gm.ai->is_main_hit = false;
					do
					{
						gm.eAttackOrder++;
						if (gm.eAttackOrder > 4)
							gm.eAttackOrder = 0;
					} while (enemyMap->ship[gm.eAttackOrder]->is_die);
				}
				else if (is_enemy_turn_end)
				{
					if (gm.allyColor == BLUE && gm.pUseSkill)
					{
						gm.skillCool = 0;
						gm.haveSkill = false;
						gm.pUseSkill = false;
						ui->skill_check->visible = false;
						ui->skill_icon->color.a = 0.3;
					}
					if(gm.enemyColor == RED)
						gm.eUseSkill = false;

					eFocusTile = nullptr;
					is_enemy_turn_end = false;
					is_player_turn = true;
					is_player_attack = true;
					do
					{
						gm.pAttackOrder++;
						if (gm.pAttackOrder > 4)
							gm.pAttackOrder = 0;
					} while (playerMap->ship[gm.pAttackOrder]->is_die);
					pointer->visible = true;
					pointer->setCenter(playerMap->pos + playerMap->ship[gm.pAttackOrder]->center() - Vec2(0, 70));
				}
			}
		}
	}

	if (world.getKeyState(VK_TAB) == 1)
	{
		world.changeScene(new DeployScene());
		return;
	}


	for (auto a = fishList.begin(); a != fishList.end(); a++)
	{
		(*a)->pos.x -= 200 * dt;

		if ((*a)->pos.x < -(*a)->rect.width())
		{
			removeChild(*a);

			a = fishList.erase(a);

			if (a == fishList.end())
				break;
		}

	}

	effectList.erase(remove_if(effectList.begin(), effectList.end(),
		[&](Animation* a){
		if (a->is_end)
			removeChild(a);
		return a->is_end;
	}), effectList.end());

	particleList.erase(remove_if(particleList.begin(), particleList.end(),
		[&](Particle* p) {
		if (p->deleting)
			removeChild(p);
		return p->deleting;
	}), particleList.end());

	bulletList.erase(remove_if(bulletList.begin(), bulletList.end(),
		[&](Bullet* b) {
		if (b->deleting)
			removeChild(b);
		return b->deleting;
	}), bulletList.end());
}


void GameScene::setFilter(Tile* tile, bool visible)
{
	if (visible)
	{
		tile->filter->setPath("image/Game/Tile/attack.png");
		tile->filter->visible = true;

		if (gm.bulletType == BARRAGE_BULLET)
		{
			for (int i = 0; i < 4; i++)
			{
				float dir = i * D3DXToRadian(90);
				int x = tile->index.x + (int)cos(dir);
				int y = tile->index.y + (int)sin(dir);

				if (9 < y || y < 0) continue;
				if (9 < x || x < 0) continue;
				if (enemyMap->tile[y][x]->is_limit) continue;

				enemyMap->tile[y][x]->filter->setPath("image/Game/Tile/attack.png");
				enemyMap->tile[y][x]->filter->visible = true;
			}
		}
		else if (gm.bulletType == FLARE_BULLET)
		{
			for (int i = -1; i <= 1; i++)
			{
				int y = tile->index.y + i;
				if (9 < y || y < 0) continue;
				for (int j = -1; j <= 1; j++)
				{
					int x = tile->index.x + j;
					if (9 < x || x < 0) continue;
					if (enemyMap->tile[y][x]->is_limit) continue;

					enemyMap->tile[y][x]->filter->setPath("image/Game/Tile/attack.png");
					enemyMap->tile[y][x]->filter->visible = true;
				}
			}
		}
	}
	else
	{
		tile->filter->visible = false;

		if (gm.bulletType == BARRAGE_BULLET)
		{
			for (int i = 0; i < 4; i++)
			{
				float dir = i * D3DXToRadian(90);
				int x = tile->index.x + (int)cos(dir);
				int y = tile->index.y + (int)sin(dir);

				if (9 < y || y < 0) continue;
				if (9 < x || x < 0) continue;
				if (enemyMap->tile[y][x]->is_limit) continue;

				enemyMap->tile[y][x]->filter->visible = false;
			}
		}
		else if (gm.bulletType == FLARE_BULLET)
		{
			for (int i = -1; i <= 1; i++)
			{
				int y = tile->index.y + i;
				if (9 < y || y < 0) continue;
				for (int j = -1; j <= 1; j++)
				{
					int x = tile->index.x + j;
					if (9 < x || x < 0)continue;
					if (enemyMap->tile[y][x]->is_limit) continue;

					enemyMap->tile[y][x]->filter->visible = false;
				}
			}
		}
	}
}


void GameScene::spawnBullet(BulletType type, Team team, Tile* targetTile, bool is_main, bool is_sub)
{
	Bullet* b;
	if (team == ALLY)
	{
		b = new Bullet(type, team, gm.allyColor);
		b->pos = playerMap->ship[gm.pAttackOrder]->center() + playerMap->pos;
		b->owner = playerMap->ship[gm.pAttackOrder];
		b->ownerMap = playerMap;
		b->targetMap = enemyMap;
		b->is_miss = gm.eUseSkill && (gm.enemyColor == BLUE);
	}
	else
	{
		b = new Bullet(type, team, gm.enemyColor);
		b->pos = enemyMap->pos + Vec2(random(200, 400), random(200, 400));
		b->owner = enemyMap->ship[gm.eAttackOrder];
		b->ownerMap = enemyMap;
		b->targetMap = playerMap;
		b->is_miss = gm.pUseSkill && (gm.allyColor == BLUE);
	}
	addChild(b);
	bulletList.push_back(b);
	b->targetPos = targetTile->center();
	b->targetTile = targetTile;
	b->is_main = is_main;
	if (is_sub)
		b->scale = Vec2(0.5, 0.5);
}