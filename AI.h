#pragma once
#include "Tile.h"

class AI
{
public:
	AI();
	~AI();

	void init();
	Tile* getTargetTile();

	Cell currentHitPos;
	Cell originHitPos;
	Cell attackPos;

	bool hit[10][10];
	float attack_dir;


	bool is_player_hit;
	bool is_current_hit;
	bool is_main_hit;

	bool attackMine;
};

