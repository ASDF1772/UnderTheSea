#include "DXUT.h"
#include "GameManager.h"


GameManager::GameManager()
{
	ai = new AI();
	pMap = new MapInfo();
	eMap = new MapInfo();

	for (int i = 0; i < 5; i++)
	{
		pShip[i] = new UnitInfo();
		eShip[i] = new UnitInfo();
	}

	for (int i = 0; i < 2; i++)
	{
		pObject[i] = new UnitInfo();
		eObject[i] = new UnitInfo();

	}
	mine = new UnitInfo();

	newGame();
}


GameManager::~GameManager()
{
	delete ai;
	delete pMap;
	delete eMap;

	for (int i = 0; i < 5; i++)
	{
		delete pShip[i];
		delete eShip[i];
	}

	for (int i = 0; i < 2; i++)
	{
		delete pObject[i];
		delete eObject[i];
	}

	delete mine;
}


void GameManager::newGame()
{
	init();
	for (int i = 0; i < 2; i++)
	{
		useDeployItem[i] = false;
		haveDeployItem[i] = true;

		haveItem[i] = true;
		useItem[i] = false;
		coolDown[i] = 5;
		coolDownOrigin[i] = 5;
	}
	allyColor = BLUE;
	enemyColor = RED;

	pAttackOrder = 0;
	eAttackOrder = 0;

	combo = 0;
	score = 0;
	ifstream ifs = ifstream("image/data.txt");
	string name;
	int tmp;
	while (ifs >> name >> tmp)
	{
		best = (best > tmp) ? best : tmp;
	}
	turn = 1;

	is_win = false;
	is_lose = false;
	pUseSkill = false;
	eUseSkill = false;

	haveSkill = true;

	skillCool = 3;
	skillCoolOrigin = 3;

	bulletType = BASIC_BULLET;
}


void GameManager::init()
{
	ai->init();
	pMap->init();
	eMap->init();
}


GameManager gm;