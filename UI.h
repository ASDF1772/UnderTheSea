#pragma once
#include "Entity.h"
#include "Sprite.h"

class UI :
	public Entity
{
public:
	UI();
	~UI();

	void update(float dt);

	Sprite* frame;
	Sprite* combo_frame;
	Sprite* filter;
	Sprite* turn_num[2];
	Sprite* combo_num[2];
	Sprite* score_num[5];
	Sprite* best_num[5];
	Sprite* unit_icon[5];
	Sprite* item_icon[2];
	Sprite* check_icon[2];
	Sprite* cool_num[2];
	Sprite* die_icon[5];
	Sprite* skill_icon;
	Sprite* skill_cool;
	Sprite* skill_check;

	int last_turn;
	int last_combo;
	int last_score;
	int last_best;
};

