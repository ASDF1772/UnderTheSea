#pragma once
#include "Entity.h"
#include "Tile.h"
#include "Ship.h"
#include "MapInfo.h"
#include "UnitInfo.h"

class StageMap :
	public Entity
{
public:
	StageMap(Team team, TeamColor teamColor);
	~StageMap();

	void init();
	Tile* getTile(Vec2 pos);

	Team team;
	TeamColor teamColor;
	Tile* tile[10][10];
	MapInfo* mapInfo;

	UnitInfo* shipInfo[5];
	Ship* ship[5];

	UnitInfo* objectInfo[2];
	Ship* object[2];

	UnitInfo* mineInfo;
	Ship* mine;
};