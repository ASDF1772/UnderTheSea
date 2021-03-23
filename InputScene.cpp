#include "DXUT.h"
#include "InputScene.h"
#include "World.h"
#include "RankingScene.h"
#include "GameManager.h"
#include "Asset.h"

InputScene::InputScene()
{
	bg = new Sprite("image/Input/bg.png");
	best = new Sprite("image/Input/best.png");
	name = new Text("", 150, D3DXCOLOR(1, 1, 1, 1));

	addChild(bg);
	addChild(best);
	addChild(name);

	best->visible = false;

	int score_tmp = gm.score;
	for (int i = 0; i < 5; i++)
	{
		score[i] = new Sprite("image/Input/Score/" + to_string(score_tmp % 10) +".png");

		addChild(score[i]);

		score[i]->pos = Vec2(708 - i * 44, 521);
		score_tmp /= 10;
	}

	name->pos = Vec2(460, 280);
	asset.sounds[L"image/Sound/win.wav"]->Stop();
	asset.sounds[L"image/Sound/lose.wav"]->Stop();
	asset.sounds[L"image/Sound/main.wav"]->Reset();

	if (gm.score == gm.best)
		best->visible = true;
}


InputScene::~InputScene()
{
}


void InputScene::update(float dt)
{
	Scene::update(dt);

	if (!asset.sounds[L"image/Sound/main.wav"]->IsSoundPlaying())
	{
		asset.sounds[L"image/Sound/main.wav"]->Play();
	}

	if (world.getKeyState(VK_RETURN) == 1)
	{
		world.changeScene(new RankingScene(name->text, gm.score));
		return;
	}

	for (int i = 0; i < 256; i++)
	{
		if (world.getKeyState(i) == 1)
		{
			if (i == VK_BACK)
			{
				if(name->text.size() > 0)
					name->text.pop_back();
			}
			else if (name->text.size() > 5)
			{
				return;
			}
			else
			{
				name->text += i;
			}
		}
	}
}