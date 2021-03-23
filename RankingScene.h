#pragma once
#include "Scene.h"
#include "Text.h"
#include "Particle.h"

typedef struct Rank
{
	string name;
	int score;
}Rank;

class RankingScene :
	public Scene
{
public:
	RankingScene(string name = "", int score = 0);
	~RankingScene();

	void update(float dt);

	ifstream ifs;
	ofstream ofs;

	Sprite* bg;
	Sprite* mine;
	Sprite* close;
	Text* Name[5];
	Text* Score[5];

	bool is_new_record;
	float paperTimer;

	list<Rank> ranks;
	vector<Particle*> particleList;
};

