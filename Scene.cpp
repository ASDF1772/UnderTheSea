#include "DXUT.h"
#include "Scene.h"


Scene::Scene():followTarget(nullptr), offset(0,0)
{
	cursor = new Sprite("image/Game/UI/cursor/basic.png");
	originPos = Vec2(0, 0);
}


Scene::~Scene()
{
	delete cursor;
}


void Scene::follow(Entity* target, Vec2 offset)
{
	followTarget = target;
	this->offset = offset;
}


void Scene::unfollow()
{
	followTarget = nullptr;
}


void Scene::shake(float duration, int count, float range)
{
	pos = originPos;
	originPos = pos;
	shakeTimer.reset(duration, count);
	shakeTimer.onTick = [=]()
	{
		pos += Vec2(random(-range, range), random(-range, range));
	};
	shakeTimer.onFinished = [=]()
	{
		pos = originPos;
	};
}


void Scene::update(float dt)
{
	Entity::update(dt);

	if (followTarget)
	{
		pos -= (followTarget->pos - offset + pos) * 0.15;
	}
}