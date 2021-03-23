#include "DXUT.h"
#include "AI.h"
#include "GameManager.h"


AI::AI() :is_player_hit(false), is_current_hit(false), is_main_hit(false), attackMine(false)
{
}


AI::~AI()
{
}


void AI::init()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			hit[i][j] = false;
		}
	}
}


Tile* AI::getTargetTile()
{
	int i = 0;
	while (1)
	{
		if (is_player_hit)
		{
			float attack_angle = random(0, 3) * D3DXToRadian(90);

			if (originHitPos.x == currentHitPos.x && originHitPos.y == currentHitPos.y)
			{
				attack_dir = attack_angle;
			}
			else
			{
				if (is_current_hit)
				{
					attack_angle = attack_dir;
				}
				else
				{
					currentHitPos = originHitPos;
					attack_angle = attack_dir -= D3DXToRadian(180);
				}
			}
			int x = (int)cos(attack_angle);
			int y = (int)sin(attack_angle);

			attackPos.x = currentHitPos.x + x;
			attackPos.y = currentHitPos.y + y;

			if (9 < attackPos.y || attackPos.y < 0)
			{
				is_current_hit = false;
				continue;
			}
			if (9 < attackPos.x || attackPos.x < 0)
			{
				is_current_hit = false;
				continue;
			}
		}
		else
		{
			attackPos.x = random(0, 9);
			attackPos.y = random(0, 9);
			//attackPos.x = 2;
			//attackPos.y = 2;
			if (attackMine && gm.pMap->haveMine)
			{
				attackPos = gm.mine->pos;
				is_player_hit = false;
				is_current_hit = false;
				attackMine = false;
			}

			originHitPos = attackPos;
		}

		if (!hit[attackPos.y][attackPos.x] && !gm.ingame->playerMap->tile[attackPos.y][attackPos.x]->is_limit)
		{
			return gm.ingame->playerMap->tile[attackPos.y][attackPos.x];
			break;
		}
		else
		{
			is_current_hit = false;
		}
		if (++i > 10)
		{
			is_player_hit = false;
			is_current_hit = false;
		}
	}
}