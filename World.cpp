#include "DXUT.h"
#include "World.h"
#include "Asset.h"
#include "LoadingScene.h"
#include "DeployScene.h"
#include "MainScene.h"
#include "IntroScene.h"
#include "EndingScene.h"


World::World(): currentScene(nullptr), slowTime(1)
{
}


World::~World()
{
}


void World::init()
{
	asset.init();
	cout.precision(3);
	D3DXCreateSprite(DXUTGetD3D9Device(), &rootSprite);
	changeScene(new LoadingScene());
	ZeroMemory(lastKeys, sizeof(lastKeys));
	ZeroMemory(currentKeys, sizeof(currentKeys));
}


void World::update(float dt)
{
	if (currentScene)
	{
		if (getKeyState(VK_F1) == 1)
		{
			slowTime = 1.5;
		}
		if (getKeyState(VK_F1) == 2)
		{
			if (slowTime < 2)
				slowTime += dt / 4;
		}
		else if (getKeyState(VK_F1) == -1)
		{
			slowTime = 1;
		}
		if (getKeyState(VK_F2) == 1)
		{
			slowTime = 0.5;
		}
		if (getKeyState(VK_F2) == 2)
		{
			if (slowTime > 0)
				slowTime -= dt / 4;
		}
		else if (getKeyState(VK_F2) == -1)
		{
			slowTime = 1;
		}
		if (getKeyState(VK_F9) == 1)
		{
			changeScene(new MainScene());
		}
		if (getKeyState(VK_F10) == 1)
		{
			changeScene(new DeployScene());
		}
		if (getKeyState(VK_F11) == 1)
		{
			changeScene(new IntroScene());
		}
		if (getKeyState(VK_F12) == 1)
		{
			cout << "asdf" << endl;
			changeScene(new EndingScene(BLUE, true));
		}

		currentScene->update(dt * slowTime);	

		currentScene->cursor->pos = world.getMousePos() + currentScene->pos;
	}
	for (int i = 0; i < 256; i++)
	{
		lastKeys[i] = currentKeys[i];
		currentKeys[i] = GetAsyncKeyState(i) & 0x8000;
	}
	GetCursorPos(&mousePoint);
	ScreenToClient(DXUTGetHWND(), &mousePoint);

	for (auto timer : timers)
		timer->update(dt);
}


void World::render()
{
	if (currentScene)
	{
		currentScene->render();
		currentScene->cursor->render();
	}
}

void World::dispose()
{
	asset.dispose();
	SAFE_RELEASE(rootSprite);
	SAFE_DELETE(currentScene);
}

void World::changeScene(Scene* scene)
{
	if (currentScene) SAFE_DELETE(currentScene);

	currentScene = scene;
}

int World::getKeyState(int vk)
{
	if (lastKeys[vk] && currentKeys[vk]) return 2;
	if (!lastKeys[vk] && currentKeys[vk]) return 1;
	if (lastKeys[vk] && !currentKeys[vk]) return -1;
	return -2;
}


Vec2 World::getMousePos()
{
	return Vec2(mousePoint.x, mousePoint.y) - currentScene->pos;
}

World world;