#include "DXUT.h"
#include "Timer.h"
#include "World.h"


Timer::Timer()
{
	onTick = []() {};
	onFinished = []() {};

	world.timers.push_back(this);
}


Timer::~Timer()
{
	world.timers.remove(this);
}


void Timer::reset(float duration, int count, bool autoStart)
{
	this->duration.first = 0;
	this->duration.second = duration;
	this->count.first = 0;
	this->count.second = count;
	active = autoStart;
}


void Timer::update(float dt)
{
	if (active)
	{
		duration.first += dt;

		if (duration.first > duration.second)
		{
			duration.first = 0;
			onTick();
			if (++count.first >= count.second)
			{
				onFinished();
				active = false;
				return;
			}
		}
	}
}