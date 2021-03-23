#include "DXUT.h"
#include "Ship.h"
#include "GameManager.h"


Ship::Ship(UnitType type, Team team, TeamColor teamColor, int dir, int size, int index)
	:type(type), team(team), teamColor(teamColor), dir(dir), size(size), index(index), hitCount(0), is_die(false), die_end(false)
{
	cell = new Cell[size];

	float rotationAngle = dir * D3DXToRadian(90);

	for (int i = 0; i < size; i++)
	{
		cell[i].x = (int)cos(rotationAngle) * i;
		cell[i].y = (int)sin(rotationAngle) * i;
	}


	if (type == SHIP)
	{
		spr = new Animation("image/Game/Unit/" + to_string(teamColor) + "/" + to_string(index) + "/" + to_string(dir) + "/", 11, 11);
		if (team == ENEMY)
			visible = false;
	}
	else if (type == OBJECT)
	{
		if (size == 1) {
			spr = new Sprite("image/Game/Unit/Object/1.png");
		}
		else {
			spr = new Sprite("image/Game/Unit/Object/" + to_string(size + dir) + ".png");
		}
	}
	else if (type == MINE)
	{
		spr = new Sprite("image/Game/Unit/mine.png");
	}


	addChild(spr);

	rect = spr->rect;
}


Ship::~Ship()
{
}


void Ship::update(float dt)
{
	Entity::update(dt);

	if (is_die && !die_end)
	{
		die_end = true;
		visible = false;

		if (team == ENEMY)
		{
			gm.ingame->ui->die_icon[index]->visible = true;
			gm.ingame->ui->unit_icon[index]->color.a = 0.5;
		}

		for (int i = 0; i < size; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int y = mapPos.y + cell[i].y + j;
				if (9 < y || y < 0) continue;

				for (int k = -1; k <= 1; k++)
				{
					int x = mapPos.x + cell[i].x + k;
					if (9 < x || x < 0) continue;

					if (team == ALLY)
					{
						if (gm.ingame->playerMap->tile[y][x]->is_limit) continue;

						gm.ingame->playerMap->tile[y][x]->setLimit();
					}
					else
					{
						if (gm.ingame->enemyMap->tile[y][x]->is_limit) continue;

						gm.ingame->enemyMap->tile[y][x]->setLimit();
					}
				}
			}
		}

		for (int i = 0; i < size; i++)
		{
			int x = mapPos.x + cell[i].x;
			int y = mapPos.y + cell[i].y;

			Animation* a = new Animation("image/Game/Effect/Explode/", 26, 30, false);
			gm.ingame->addChild(a);
			gm.ingame->effectList.push_back(a);

			if (team == ALLY)
			{
				gm.ingame->playerMap->tile[y][x]->debris->visible = true;
				gm.ingame->playerMap->tile[y][x]->is_smoke = false;
				a->setCenter(gm.ingame->playerMap->tile[y][x]->center());
			}
			else
			{
				gm.ingame->enemyMap->tile[y][x]->debris->visible = true;
				gm.ingame->enemyMap->tile[y][x]->is_smoke = false;
				a->setCenter(gm.ingame->enemyMap->tile[y][x]->center());
			}
		}
	}
}