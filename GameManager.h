#pragma once
#include "GameScene.h"
#include "DeployScene.h"
#include "MapInfo.h"
#include "UnitInfo.h"
#include "AI.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void newGame();
	void init();
	
	AI* ai;

	DeployScene* indeploy;
	GameScene* ingame;

	MapInfo* pMap;
	MapInfo* eMap;

	UnitInfo* pShip[5];
	UnitInfo* eShip[5];

	UnitInfo* mine;
	UnitInfo* pObject[2];
	UnitInfo* eObject[2];

	int pAttackOrder;
	int eAttackOrder;

	int combo;
	int score;
	int best;
	int turn;

	bool is_win;
	bool is_lose;

	bool pUseSkill;
	bool eUseSkill;

	bool haveSkill;

	int skillCool;
	int skillCoolOrigin;

	bool useDeployItem[2];
	bool haveDeployItem[2];

	bool haveItem[2];
	bool useItem[2];
	int coolDown[2];
	int coolDownOrigin[2];

	BulletType bulletType;
	TeamColor allyColor;
	TeamColor enemyColor;
};


extern GameManager gm;