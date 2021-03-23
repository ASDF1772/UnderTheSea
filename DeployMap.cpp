#include "DXUT.h"
#include "DeployMap.h"


DeployMap::DeployMap(TeamColor teamColor) :teamColor(teamColor), mine(nullptr)
{
}


DeployMap::~DeployMap()
{
}


void DeployMap::init()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			tile[i][j] = new DeployTile();
			addChild(tile[i][j]);
			tile[i][j]->pos = Vec2(j, i) * DEPLOY_SIZE;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		ship[i] = new Model(SHIP, teamColor, i / 2 + 2, i);
		addChild(ship[i]);
	}
	ship[0]->originPos = ship[0]->pos = Vec2(615, 60);
	ship[1]->originPos = ship[1]->pos = Vec2(715, 60);
	ship[2]->originPos = ship[2]->pos = Vec2(615, 175);
	ship[3]->originPos = ship[3]->pos = Vec2(730, 180);
	ship[4]->originPos = ship[4]->pos = Vec2(593, 350);

	for (int i = 0; i < 2; i++)
	{
		int dir = random(0, 1);
		object[i] = new Model(OBJECT, teamColor, i + 1, i);
		addChild(object[i]);
		object[i]->rotation = dir * D3DXToRadian(90);
		object[i]->setCell();
		objectInfo[i]->size = i + 1;

		while (1)
		{
			int x = random(0, 9);
			int y = random(0, 9);

			if (checkDeploy(object[i], x, y))
			{
				object[i]->pos = Vec2(x, y) * DEPLOY_SIZE;
				object[i]->is_deployed = true;
				setTile(object[i], OBJECT_TILE);
				objectInfo[i]->dir = dir;
				objectInfo[i]->pos = Cell(x, y);
				break;
			}
		}

	}
}


void DeployMap::setTile(Model* ship, TileType type)
{
	int x_pos = ship->offsetCenter().x / DEPLOY_SIZE;
	int y_pos = ship->offsetCenter().y / DEPLOY_SIZE;

	for (int i = 0; i < ship->size; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int y = y_pos + ship->cell[i].y + j;
			if (9 < y || y < 0) continue;

			for (int k = -1; k <= 1; k++)
			{
				int x = x_pos + ship->cell[i].x + k;
				if (9 < x || x < 0) continue;

				if (type == BASIC_TILE)
				{
					mapInfo->type[y][x] = BASIC_TILE;
					tile[y][x]->filter->visible = false;
				}
				else
				{
					mapInfo->type[y][x] = LIMIT_TILE;
					tile[y][x]->filter->visible = true;
				}
			}
		}
	}
	
	for (int i = 0; i < ship->size; i++)
	{
		int y = y_pos + ship->cell[i].y;
		int x = x_pos + ship->cell[i].x;

		mapInfo->type[y][x] = type;
	}
}


void DeployMap::setTile(Model* ship, TileType type, int x_pos, int y_pos)
{
	for (int i = 0; i < ship->size; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int y = y_pos + ship->cell[i].y + j;
			if (9 < y || y < 0) continue;

			for (int k = -1; k <= 1; k++)
			{
				int x = x_pos + ship->cell[i].x + k;
				if (9 < x || x < 0) continue;

				if (type == BASIC_TILE)
				{
					mapInfo->type[y][x] = BASIC_TILE;
					tile[y][x]->filter->visible = false;
				}
				else
				{
					mapInfo->type[y][x] = LIMIT_TILE;
					tile[y][x]->filter->visible = true;
				}
			}
		}
	}

	for (int i = 0; i < ship->size; i++)
	{
		int y = y_pos + ship->cell[i].y;
		int x = x_pos + ship->cell[i].x;

		mapInfo->type[y][x] = type;
	}
}


bool DeployMap::checkDeploy(Model* ship, int x_pos, int y_pos)
{
	for (int i = 0; i < ship->size; i++)
	{
		int y = y_pos + ship->cell[i].y;
		int x = x_pos + ship->cell[i].x;

		if (9 < y || y < 0) return false;
		if (9 < x || x < 0) return false;

		if (ship->type == MINE)
		{
			if (mapInfo->type[y][x] == UNIT_TILE || mapInfo->type[y][x] == OBJECT_TILE) return false;
		}
		else
		{
			if (mapInfo->type[y][x] != BASIC_TILE) return false;
		}
	}

	return true;
}