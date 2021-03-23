#include "DXUT.h"
#include "MapInfo.h"


MapInfo::MapInfo()
{
	init();
}


MapInfo::~MapInfo()
{
}


void MapInfo::init()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			type[i][j] = BASIC_TILE;
		}
	}
	for (int i = 0; i < 2; i++)
	{
		haveObject[i] = true;
	}
	haveMine = false;
	leftTile = 14;
}