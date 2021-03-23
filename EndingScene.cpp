#include "DXUT.h"
#include "EndingScene.h"
#include "World.h"
#include "InputScene.h"
#include "Asset.h"


EndingScene::EndingScene(TeamColor teamColor, bool is_win) :is_win(is_win)
{
	ani = new Animation("image/Ending/"+to_string(teamColor)+"/", 31, 11, 0);
	win = new Particle("image/Ending/" + to_string(teamColor) + ".png", FADE, 0.7, 1, true);

	addChild(ani);
	addChild(win);

	updateEnabled = false;
	asset.sounds[L"image/Sound/ingame.wav"]->Stop();
	asset.sounds[L"image/Sound/win.wav"]->Reset();
	asset.sounds[L"image/Sound/lose.wav"]->Reset();
}


EndingScene::~EndingScene()
{
}


void EndingScene::update(float dt)
{
	Scene::update(dt);
	if (is_win)
	{
		if (!asset.sounds[L"image/Sound/win.wav"]->IsSoundPlaying())
		{
			asset.sounds[L"image/Sound/win.wav"]->Play();
		}
	}
	else
	{
		if (!asset.sounds[L"image/Sound/lose.wav"]->IsSoundPlaying())
		{
			asset.sounds[L"image/Sound/lose.wav"]->Play();
		}
	}

	if (world.getKeyState(VK_LBUTTON) == 1)
	{
		if (ani->is_end)
		{
			world.changeScene(new InputScene());
			return;
		}
		else
		{
			updateEnabled = true;
		}
	}
}