#pragma once

#define MAP_HEIGHT 10
#define MAP_WIDTH 10
#define TILE_HEIGHT 41
#define TILE_WIDTH 57
#define DEPLOY_SIZE 55
#define Y_INTERCEPT MAP_HEIGHT * TILE_HEIGHT / 2

enum Team
{
	ALLY,
	ENEMY
};


enum TeamColor
{
	BLUE,
	RED
};


enum TileType
{
	BASIC_TILE,
	UNIT_TILE,
	LIMIT_TILE,
	MINE_TILE,
	OBJECT_TILE
};


enum UnitType
{
	SHIP,
	OBJECT,
	MINE
};

enum BulletType
{
	BASIC_BULLET,
	BARRAGE_BULLET,
	FLARE_BULLET
};

enum ParticleType
{
	FADE,
	BUBBLE,
	BUBBLE2,
	SMOKE,
	PAPER
};