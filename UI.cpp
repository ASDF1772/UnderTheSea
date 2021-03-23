#include "DXUT.h"
#include "UI.h"
#include "World.h"
#include "GameManager.h"


UI::UI() :last_turn(0), last_score(0), last_combo(0)
{
	filter = new Sprite("image/Game/UI/filter.png");
	addChild(filter);
	filter->color.a = 0.7;

	frame = new Sprite("image/Game/UI/frame.png");
	addChild(frame);

	combo_frame = new Sprite("image/Game/UI/combo.png");
	addChild(combo_frame);

	for (int i = 0; i < 2; i++)
	{
		turn_num[i] = new Sprite("image/Game/UI/Turn/0.png");
		combo_num[i] = new Sprite("image/Game/UI/Turn/0.png");

		addChild(turn_num[i]);
		addChild(combo_num[i]);

		turn_num[i]->pos = Vec2(56 - i * 33, 23);
		combo_num[i]->pos = Vec2(634 - i * 33, 62);
	}

	for (int i = 0; i < 2; i++)
	{
		item_icon[i] = new Sprite("image/Game/UI/Item/" + to_string(i) + ".png");
		check_icon[i] = new Sprite("image/Game/UI/check.png");
		cool_num[i] = new Sprite("image/Game/UI/Cool/0.png");

		addChild(item_icon[i]);
		addChild(check_icon[i]);
		addChild(cool_num[i]);

		item_icon[i]->pos = Vec2(50 + i * 120, 604);
		check_icon[i]->pos = Vec2(100 + i * 116, 634);
		cool_num[i]->pos = Vec2(70 + i * 116, 618);
		check_icon[i]->visible = false;
	}

	int best_tmp = gm.best;
	for (int i = 0; i < 5; i++)
	{
		score_num[i] = new Sprite("image/Game/UI/Score/0.png");
		best_num[i] = new Sprite("image/Game/UI/Best/"+to_string(best_tmp % 10)+".png");

		addChild(score_num[i]);
		addChild(best_num[i]);

		score_num[i]->pos = Vec2(1210 - i * 34, 54);
		best_num[i]->pos = Vec2(1194 - i * 24, 129);
		best_tmp /= 10;
	}

	for (int i = 0; i < 5; i++)
	{
		unit_icon[i] = new Sprite("image/Game/UI/Unit/" + to_string(gm.enemyColor) + "/" + to_string(i) + ".png");
		die_icon[i] = new Sprite("image/Game/UI/die.png");

		addChild(unit_icon[i]);
		addChild(die_icon[i]);

		unit_icon[i]->setCenter(Vec2(900 + i * 70, 620));
		die_icon[i]->setCenter(Vec2(900 + i * 70, 620));
		die_icon[i]->visible = false;
	}

	skill_icon = new Sprite("image/Game/UI/Skill/" + to_string(gm.allyColor) + ".png");
	skill_check = new Sprite("image/Game/UI/check.png");
	skill_cool = new Sprite("image/Game/UI/Cool/0.png");

	addChild(skill_icon);
	addChild(skill_check);
	addChild(skill_cool);

	skill_icon->pos = Vec2(31, 428);
	skill_check->pos = Vec2(96, 485);
	skill_cool->pos = Vec2(70, 461);

	skill_check->visible = false;
	skill_cool->visible = false;
}


UI::~UI()
{
}


void UI::update(float dt)
{
	Entity::update(dt);

	if (gm.turn != last_turn)
	{
		int turn_tmp = last_turn = gm.turn;

		for (int i = 0; i < 2; i++)
		{
			turn_num[i]->setPath("image/Game/UI/Turn/" + to_string(turn_tmp % 10) + ".png");

			turn_tmp /= 10;
		}
	}

	if (gm.score < 0)
		gm.score = 0;
	else if (gm.score > 99999)
		gm.score = 99999;

	if (gm.score != last_score)
	{
		int score_tmp = last_score = gm.score;

		for (int i = 0; i < 5; i++)
		{
			score_num[i]->setPath("image/Game/UI/Score/" + to_string(score_tmp % 10) + ".png");

			score_tmp /= 10;
		}
	}

	if (gm.score > gm.best)
	{
		int best_tmp = last_score = gm.best = gm.score;

		for (int i = 0; i < 5; i++)
		{
			best_num[i]->setPath("image/Game/UI/Best/" + to_string(best_tmp % 10) + ".png");

			best_tmp /= 10;
		}
	}


	if (gm.combo != last_combo)
	{
		int combo_tmp = last_combo = gm.combo;

		for (int i = 0; i < 2; i++)
		{
			combo_num[i]->setPath("image/Game/UI/Turn/" + to_string(combo_tmp % 10) + ".png");

			combo_tmp /= 10;
		}
	}

	if (gm.ingame->is_player_turn)
	{
		for (int i = 0; i < 2; i++)
		{
			if (item_icon[i]->rectWithPos().contain(world.getMousePos()))
			{
				if (gm.haveItem[i])
				{
					if (world.getKeyState(VK_LBUTTON) == 1)
					{
						if (gm.useItem[i])
						{
							gm.bulletType = BASIC_BULLET;
							check_icon[i]->visible = false;
							gm.useItem[i] = false;
						}
						else
						{
							for (int j = 0; j < 2; j++)
							{
								gm.useItem[j] = false;
								check_icon[j]->visible = false;
							}
							check_icon[i]->visible = true;
							gm.useItem[i] = true;
							if (i == 0)
								gm.bulletType = BARRAGE_BULLET;
							else
								gm.bulletType = FLARE_BULLET;
						}
					}
				}
			}
		}

		if (skill_icon->rectWithPos().contain(world.getMousePos()))
		{
			if (gm.haveSkill)
			{
				if (world.getKeyState(VK_LBUTTON) == 1)
				{
					if (gm.pUseSkill)
					{
						skill_check->visible = false;
						gm.pUseSkill = false;
					}
					else
					{
						skill_check->visible = true;
						gm.pUseSkill = true;
					}
				}
			}
		}
	}
	for (int i = 0; i < 2; i++)
	{
		int cool_tmp = gm.coolDownOrigin[i] - gm.coolDown[i];
		if (cool_tmp <= 0)
		{
			gm.haveItem[i] = true;
			item_icon[i]->color.a = 1;
			cool_num[i]->visible = false;
		}
		else
		{
			cool_num[i]->visible = true;
			cool_num[i]->setPath("image/Game/UI/Cool/" + to_string(cool_tmp) + ".png");
		}
	}
	int cool_tmp = gm.skillCoolOrigin - gm.skillCool;

	if (cool_tmp <= 0)
	{
		gm.haveSkill = true;
		skill_icon->color.a = 1;
		skill_cool->visible = false;
	}
	else
	{
		skill_cool->visible = true;
		skill_cool->setPath("image/Game/UI/Cool/" + to_string(cool_tmp) + ".png");
	}
}