#include "DXUT.h"
#include "StageMap.h"


StageMap::StageMap(Team team, TeamColor teamColor) :team(team), teamColor(teamColor)
{
}


StageMap::~StageMap()
{
}


void StageMap::init()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			tile[i][j] = new Tile(mapInfo->type[i][j], teamColor);
			addChild(tile[i][j]);
			tile[i][j]->pos = Vec2((j - i - 1 + MAP_HEIGHT) * TILE_WIDTH / 2, (j + i) * TILE_HEIGHT / 2);
			tile[i][j]->index = Cell(j, i);
		}
	}
	
	for (int i = 0; i < 2; i++)
	{
		if (mapInfo->haveObject[i])
		{
			object[i] = new Ship(OBJECT, team, teamColor, objectInfo[i]->dir, i + 1, i);
			addChild(object[i]);
			object[i]->mapPos = objectInfo[i]->pos;

			float x = objectInfo[i]->pos.x + object[i]->cell[i].x / 2.0;
			float y = objectInfo[i]->pos.y + object[i]->cell[i].y / 2.0;

			object[i]->setCenter(Vec2((x - y + MAP_HEIGHT) * TILE_WIDTH / 2, (x + y + 1) * TILE_HEIGHT / 2));


			for (int j = 0; j < i + 1; j++)
			{
				int x_pos = objectInfo[i]->pos.x + object[i]->cell[j].x;
				int y_pos = objectInfo[i]->pos.y + object[i]->cell[j].y;

				tile[y_pos][x_pos]->is_limit = true;
			}
		}
	}

	if (mapInfo->haveMine)
	{
		mine = new Ship(MINE, team, teamColor, mineInfo->dir, 1, 0);
		addChild(mine);
		mine->mapPos = mineInfo->pos;

		float x = mineInfo->pos.x;
		float y = mineInfo->pos.y;

		tile[(int)y][(int)x]->ship = mine;

		mine->setCenter(Vec2((x - y + MAP_HEIGHT) * TILE_WIDTH / 2, (x + y + 1) * TILE_HEIGHT / 2));
	}

	for (int i = 0; i < 5; i++)
	{
		ship[i] = new Ship(SHIP, team, teamColor, shipInfo[i]->dir, i / 2 + 2, i);
		addChild(ship[i]);
		ship[i]->mapPos = shipInfo[i]->pos;
		float x = shipInfo[i]->pos.x + ship[i]->cell[i / 2 + 1].x / 2.0;
		float y = shipInfo[i]->pos.y + ship[i]->cell[i / 2 + 1].y / 2.0;

		ship[i]->setCenter(Vec2((x - y + MAP_HEIGHT) * TILE_WIDTH / 2, (x + y) * TILE_HEIGHT / 2));

		for (int j = 0; j < i / 2 + 2; j++)
		{
			int x_pos = shipInfo[i]->pos.x + ship[i]->cell[j].x;
			int y_pos = shipInfo[i]->pos.y + ship[i]->cell[j].y;

			tile[y_pos][x_pos]->ship = ship[i];
		}
	}
}


Tile* StageMap::getTile(Vec2 pos)
{
	pos -= this->pos;

	float ratio = (float)TILE_HEIGHT / (float)TILE_WIDTH;

	int y = (pos.y - ratio * pos.x + Y_INTERCEPT) / TILE_HEIGHT;
	if (pos.y - ratio * pos.x + Y_INTERCEPT < 0)
		y--;

	if (9 < y || y < 0) return nullptr;

	int x = (pos.y + ratio * pos.x - Y_INTERCEPT) / TILE_HEIGHT;
	if (pos.y + ratio * pos.x - Y_INTERCEPT < 0)
		x--;

	if (9 < x || x < 0) return nullptr;

	if (tile[y][x]->is_limit)
		return nullptr;

	return tile[y][x];
}