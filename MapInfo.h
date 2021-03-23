#pragma once
#include "Define.h"

class MapInfo
{
public:
	MapInfo();
	~MapInfo();

	void init();

	TileType type[10][10];
	bool haveObject[2];
	bool haveMine;
	int leftTile;
};