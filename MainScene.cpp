#include "DXUT.h"
#include "MainScene.h"
#include "World.h"
#include "GameManager.h"
#include "RankingScene.h"
#include "SelectScene.h"
#include "InfoScene.h"
#include "HowToScene.h"
#include "Asset.h"


MainScene::MainScene() :bubbleTimer(0), buttonTimer(0)
{
	lastMousePos = Vec2(0, 0);
	renderChildrenEnabled = false;
	gm.newGame();

	bg = new Animation("image/Main/ani/", 41, 10);
	logo = new Sprite("image/Main/logo.png");

	addChild(bg);
	addChild(logo);

	for (int i = 0; i < 5; i++)
	{
		button[i] = new Sprite("image/Main/Button/" + to_string(i) + ".png");
		addChild(button[i]);
		button[i]->scaleCenter = button[i]->rect.center();
	}
	button[0]->pos = Vec2(1081, 318);
	button[1]->pos = Vec2(1025, 393);
	button[2]->pos = Vec2(1081, 468);
	button[3]->pos = Vec2(998, 543);
	button[4]->pos = Vec2(1081, 618);
}


MainScene::~MainScene()
{
}


void MainScene::render()
{
	Scene::render();
	bg->render();

	for (Particle* p : particleList)
		p->render();

	logo->render();
	for (int i = 0; i < 5; i++)
	{
		button[i]->render();
	}
}


void MainScene::update(float dt)
{
	Scene::update(dt);

	DXUT_PlaySoundLooping(asset.sounds[L"image/Sound/main.wav"]);

	for (int i = 0; i < (int)distance(lastMousePos, world.getMousePos()) / 25; i++)
	{
		Particle* p = new Particle("image/Game/Effect/bubble.png", BUBBLE2);
		addChild(p);
		particleList.push_back(p);
		p->scaleCenter = p->rect.center();
		p->scale = Vec2(1, 1) * random(0.4f, 0.8f);
		p->pos = world.getMousePos() + Vec2(random(-20, 20), random(-20, 20));
		p->color.a = 0.7;
		p->moveAngle = D3DXToRadian(270);
	}
	lastMousePos = world.getMousePos();

	bubbleTimer += dt;

	if (bubbleTimer > 1)
	{
		bubblePos = random(0, 1280);
		spawnTimer.reset(0.2, 6);
		spawnTimer.onTick = [=]()
		{
			if (!random(0, 5)) return;

			Particle* p = new Particle("image/Game/Effect/bubble.png", BUBBLE);
			addChild(p);
			particleList.push_back(p);
			p->scaleCenter = p->rect.center();
			p->scale = Vec2(1, 1) * random(0.6f, 1.2f);
			p->pos = Vec2(bubblePos + random(-30, 30), 720);
			p->color.a = 0.7;
		};
		bubbleTimer = 0;
	}

	for (int i = 0; i < 5; i++)
	{
		if (button[i]->rectWithPos().contain(world.getMousePos()))
		{
			if (button[i]->scale == Vec2(1, 1))
			{
				button[i]->scale = Vec2(1.1, 1.1);
				for (int j = 0; j < 30; j++)
				{
					Particle* p = new Particle("image/Game/Effect/bubble.png", BUBBLE2);
					addChild(p);
					particleList.push_back(p);
					p->scaleCenter = p->rect.center();
					p->scale = Vec2(1, 1) * random(0.4f, 0.8f);
					p->pos = button[i]->center() + Vec2(random(-30, 30), random(-20, 20));
					p->color.a = 0.7;
					p->moveAngle = D3DXToRadian(random(200, 340));
				}
			}
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				if (i == 0)
				{
					world.changeScene(new SelectScene());
					return;
				}
				else if (i == 1)
				{
					world.changeScene(new HowToScene());
					return;
				}
				else if (i == 2)
				{
					world.changeScene(new InfoScene());
					return;
				}
				else if (i == 3)
				{
					world.changeScene(new RankingScene());
					return;
				}
				else if (i == 4)
				{
					PostQuitMessage(0);
					return;
				}
			}
		}
		else
		{
			button[i]->scale = Vec2(1, 1);
			buttonTimer = 0;
		}
	}

	particleList.erase(remove_if(particleList.begin(), particleList.end(),
		[&](Particle* p) {
		if (p->deleting)
			removeChild(p);
		return p->deleting;
	}), particleList.end());
}