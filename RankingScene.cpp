#include "DXUT.h"
#include "RankingScene.h"
#include "World.h"
#include "MainScene.h"
#include "Asset.h"


RankingScene::RankingScene(string name, int score) : is_new_record(false), paperTimer(0)
{
	bg = new Sprite("image/Ranking/bg.png");
	addChild(bg);

	mine = new Sprite("image/Ranking/mine.png");
	addChild(mine);
	mine->visible = false;

	close = new Sprite("image/Ranking/close.png");
	addChild(close);
	close->scaleCenter = close->rect.center();
	close->pos = Vec2(1106, 35);

	ifs = ifstream("image/data.txt");
	ofs = ofstream("image/data.txt", ios::app | ios::end);

	if (name != "")
	{
		ofs << name << ' ' << score << endl;
	}
	ofs.close();

	Rank r;
	while (ifs >> r.name >> r.score)
	{
		ranks.push_back(r);
	}

	ranks.sort([](Rank r1, Rank r2)
	{
		return r1.score > r2.score;
	});

	for (int i = 0; i < 5; i++)
	{
		Name[i] = new Text("", 60, D3DXCOLOR(1, 1, 1, 1));
		Score[i] = new Text("", 60, D3DXCOLOR(1, 1, 1, 1));

		addChild(Name[i]);
		addChild(Score[i]);

		Name[i]->pos = Vec2(330, 188 + i * 96);
		Score[i]->pos = Vec2(800, 188 + i * 96);
	}

	int i = 0;
	for (auto iter = ranks.begin(); iter != ranks.end(); iter++)
	{
		Name[i]->text = iter->name;
		Score[i]->text = to_string(iter->score);
		if (iter->name == name && iter->score == score)
		{
			mine->visible = true;
			mine->pos = Vec2(122, 156 + i * 96);
			if (i == 0)
				is_new_record = true;
		}
		i++;
		if (i > 4) break;
	}
}


RankingScene::~RankingScene()
{
}


void RankingScene::update(float dt)
{
	Scene::update(dt);
	if (!asset.sounds[L"image/Sound/main.wav"]->IsSoundPlaying())
	{
		asset.sounds[L"image/Sound/main.wav"]->Play();
	}

	if (close->rectWithPos().contain(world.getMousePos()))
	{
		close->scale = Vec2(1.1, 1.1);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			world.changeScene(new MainScene());
			return;
		}
	}
	else
	{
		close->scale = Vec2(1, 1);
	}

	if (is_new_record)
	{
		paperTimer += dt;

		if (paperTimer > 0.1)
		{
			Particle* p = new Particle("image/Ending/Particle/" + to_string(random(0, 3)) + ".png", PAPER);
			addChild(p);
			particleList.push_back(p);
			p->pos = Vec2(random(0, 1280), -100);
			p->rotationCenter = p->rect.center();
			paperTimer = 0;
		}
	}

	particleList.erase(remove_if(particleList.begin(), particleList.end(),
		[&](Particle* p) {
		if (p->deleting)
			removeChild(p);
		return p->deleting;
	}), particleList.end());
}