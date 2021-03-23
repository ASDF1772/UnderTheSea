#pragma once
#include "Entity.h"
#include "DeployTile.h"
#include "Model.h"
#include "MapInfo.h"
#include "UnitInfo.h"

class DeployMap :
	public Entity
{
public:
	DeployMap(TeamColor teamColor);
	~DeployMap();

	void init();
	void setTile(Model* ship, TileType type);
	void setTile(Model* ship, TileType type, int x_pos, int y_pos);
	bool checkDeploy(Model* ship, int x_pos, int y_pos);

	TeamColor teamColor;
	DeployTile* tile[10][10];
	MapInfo* mapInfo;
	Model* ship[5];

	UnitInfo* objectInfo[2];
	Model* object[2];

	UnitInfo* mineInfo;
	Model* mine;
};