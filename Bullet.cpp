#include "DXUT.h"
#include "Bullet.h"
#include "GameManager.h"
#include "Asset.h"


Bullet::Bullet(BulletType type, Team team, TeamColor teamColor) 
	:type(type), team(team), teamColor(teamColor), angleWithTarget(0), speed(300), is_main(true), bubbleTimer(0), is_miss(false),
	away(false), missTimer(0), is_increase(false)
{
	if (type == BASIC_BULLET) {
		ani = new Animation("image/Game/Bullet/Basic/" + to_string(teamColor) + "/", 11, 11);
	}
	else if (type == FLARE_BULLET) {
		ani = new Animation("image/Game/Bullet/Flare/", 11, 11);
	}
	else if (type == BARRAGE_BULLET) {
		ani = new Animation("image/Game/Bullet/Barrage/", 11, 11);
	}

	addChild(ani);
	rect = ani->rect;
	scaleCenter = rect.center();
	rotationCenter = rect.center();
}


Bullet::~Bullet()
{
}


void Bullet::update(float dt)
{
	Entity::update(dt);

	bubbleTimer += dt;
	
	if (bubbleTimer > 0.1)
	{
		for (int i = 0; i < 3; i++)
		{
			Particle* p = new Particle("image/Game/Effect/bubble.png", BUBBLE2);
			gm.ingame->addChild(p);
			gm.ingame->particleList.push_back(p);
			p->scaleCenter = p->rect.center();
			p->scale = Vec2(1, 1) * random(0.4f, 0.8f);
			p->setCenter(center() - Vec2(cos(angleWithTarget), sin(angleWithTarget)) * 20 + Vec2(random(-10, 10), random(-10, 10)));
			p->moveAngle = D3DXToRadian(540) - angleWithTarget;
		}
		bubbleTimer = 0;
	}


	if (is_miss)
	{
		if (missTimer == 0)
		{
			angleWithTarget = angle(center(), targetPos);
			targetPos = Vec2(-1000, -1000);
			is_increase = random(0, 1);
		}
		if (is_increase)
			angleWithTarget += dt;
		else
			angleWithTarget -= dt;
		missTimer += dt;
		speed -= 400 * dt;

		if (missTimer > 4)
		{
			deleting = true;
			if (is_main)
			{
				if (team == ALLY)
				{
					gm.ingame->is_player_turn = false;
					gm.ingame->is_player_turn_end = true;
					gm.ingame->setFilter(targetTile, false);
				}
				else
				{
					gm.score--;
					gm.turn++;
					gm.coolDown[0]++;
					gm.coolDown[1]++;
					gm.skillCool++;
					gm.ingame->is_enemy_turn = false;
					gm.ingame->is_enemy_turn_end = true;
				}
			}
		}
	}
	else
	{
		angleWithTarget = angle(center(), targetPos);
	}
	speed += 500 * dt;
	pos += Vec2(cos(angleWithTarget), sin(angleWithTarget)) * speed * dt;

	rotation = angleWithTarget;


	if (distance(center(), targetPos) < 30)
	{
		deleting = true;

		if (type == BASIC_BULLET)
		{
			explode(targetTile, targetPos);
		}
		else if (type == BARRAGE_BULLET)
		{
			explode(targetTile, targetPos);
		}
		else if (type == FLARE_BULLET)
		{
			asset.sounds[L"image/Sound/explode.wav"]->Reset();
			asset.sounds[L"image/Sound/explode.wav"]->Play();
			Animation* e = new Animation("image/Game/Effect/Explode/", 26, 30, false);
			gm.ingame->addChild(e);
			gm.ingame->effectList.push_back(e);
			e->setCenter(targetPos);

			for (int i = -1; i <= 1; i++)
			{
				int y = targetTile->index.y + i;
				if (9 < y || y < 0) continue;
				for (int j = -1; j <= 1; j++)
				{
					int x = targetTile->index.x + j;
					if (9 < x || x < 0)continue;
					if (targetMap->tile[y][x]->is_limit) continue;

					targetMap->tile[y][x]->filter->visible = false;

					if (targetMap->tile[y][x]->ship)
					{
						targetMap->tile[y][x]->mark->visible = true;
					}
				}
			}
		}

		if (is_main)
		{
			if (team == ALLY)
			{
				gm.ingame->is_player_turn = false;
				gm.ingame->is_player_turn_end = true;
			}
			else
			{
				gm.score--;
				gm.turn++;
				gm.coolDown[0]++;
				gm.coolDown[1]++;
				gm.skillCool++;
				gm.ingame->is_enemy_turn = false;
				gm.ingame->is_enemy_turn_end = true;
			}
		}
	}
}


void Bullet::explode(Tile* targetTile, Vec2 targetPos)
{
	Animation* e1;

	if(targetTile->type == UNIT_TILE)
	{
		e1 = new Animation("image/Game/Effect/Explode/", 26, 30, false);

		Animation* e2 = new Animation("image/Game/Effect/Debris/"+to_string(!teamColor)+"/", 26, 30, false);
		gm.ingame->addChild(e2);
		gm.ingame->effectList.push_back(e2);
		e2->setCenter(targetTile->center());

		targetTile->mark->visible = false;
		targetMap->mapInfo->leftTile--;
		targetTile->ship->hitCount++;
		targetTile->is_smoke = true;

		if (team == ENEMY)
		{
			gm.ai->currentHitPos = targetTile->index;
			gm.ai->is_player_hit = true;
			gm.ai->is_current_hit = true;
			gm.score -= 5;

			if (is_main)
			{
				gm.ai->is_main_hit = true;
			}
			else
			{
				if (gm.ai->is_main_hit)
				{
					if (gm.ai->currentHitPos.x > gm.ai->originHitPos.x)
					{
						int x_tmp = gm.ai->currentHitPos.x;
						gm.ai->currentHitPos.x = gm.ai->originHitPos.x;
						gm.ai->originHitPos.x = x_tmp;
					}
					if (gm.ai->currentHitPos.y > gm.ai->currentHitPos.y)
					{
						int y_tmp = gm.ai->currentHitPos.y;
						gm.ai->currentHitPos.y = gm.ai->originHitPos.y;
						gm.ai->originHitPos.y = y_tmp;
					}
					int x = gm.ai->currentHitPos.x - gm.ai->originHitPos.x;
					int y = gm.ai->currentHitPos.y - gm.ai->originHitPos.y;

					if (x == 0)
					{
						if (y < 0)
							y = -1;
						else
							y = 1;
						gm.ai->attack_dir = asin(y);
					}
					else if (y == 0)
					{
						if (x < 0)
							x = -1;
						else
							x = 1;
						gm.ai->attack_dir = acos(x);
					}
				}
				else
				{
					gm.ai->originHitPos = targetTile->index;
				}
			}
		}
		else
		{
			gm.score += 100 + gm.combo * 10;

			Particle* p = new Particle("image/Game/UI/plus.png", FADE, 0.5);
			gm.ingame->addChild(p);
			gm.ingame->particleList.push_back(p);
			p->pos = targetTile->center() + Vec2(20, -20);

			int score_tmp = 100 + gm.combo * 10;
			for (int i = 0; i < 3; i++)
			{
				Particle* s = new Particle("image/Game/UI/Plus/" + to_string(score_tmp % 10) + ".png", FADE, 0.5);
				gm.ingame->addChild(s);
				gm.ingame->particleList.push_back(s);
				s->pos = targetTile->center() + Vec2(70 - i * 15, -25);
				score_tmp /= 10;
			}

			if (is_main)
			{
				gm.combo++;
			}
		}


		if (targetTile->ship->hitCount >= targetTile->ship->size)
		{
			targetTile->ship->is_die = true;
			if (targetMap->mapInfo->leftTile <= 0)
			{
				if (team == ALLY)
				{
					gm.is_win = true;
				}
				else
				{
					gm.is_lose = true;
				}
			}

			Particle* p;

			if (team == ENEMY)
			{
				gm.ai->is_player_hit = false;
				gm.ai->is_current_hit = false;
				 p = new Particle("image/Game/UI/Text/0.png", FADE, 1, 1, true);
			}
			else
			{
				p = new Particle("image/Game/UI/Text/1.png", FADE, 1, 1, true);
			}
			gm.ingame->addChild(p);
			gm.ingame->particleList.push_back(p);
		}
	}
	else
	{
		if (team == ENEMY && is_main)
		{
			gm.ai->currentHitPos = gm.ai->attackPos;
			gm.ai->is_current_hit = false;
		}
		else
		{
			if (is_main)
			{
				gm.combo = 0;
			}
		}

		if (targetTile->type == BASIC_TILE)
		{
			e1 = new Animation("image/Game/Effect/Sand/", 26, 30, false);
		}
		else if (targetTile->type == MINE_TILE)
		{
			e1 = new Animation("image/Game/Effect/Explode/", 26, 30, false);

			if (team == ENEMY)
			{
				for (int i = 0; i < owner->size; i++)
				{
					int x = owner->mapPos.x + owner->cell[i].x;
					int y = owner->mapPos.y + owner->cell[i].y;

					ownerMap->tile[y][x]->mark->visible = true;
					targetTile->ship->visible = false;
				}
				gm.pMap->haveMine = false;
			}
		}
		else
		{
			e1 = new Animation("image/Game/Effect/Sand/", 26, 30, false);
		}
	}

	asset.sounds[L"image/Sound/explode.wav"]->Reset();
	asset.sounds[L"image/Sound/explode.wav"]->Play();

	e1->setCenter(targetTile->center());
	gm.ingame->addChild(e1);
	gm.ingame->effectList.push_back(e1);

	gm.ingame->shake(0.001, 15, 5);

	if(!targetTile->is_limit)
		targetTile->setLimit();
}