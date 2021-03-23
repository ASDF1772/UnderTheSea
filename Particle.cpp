#include "DXUT.h"
#include "Particle.h"


Particle::Particle(string path, ParticleType type, float fadeSpeed, float maxAlpha, bool isFadeIn)
	:Sprite(path), type(type), fadeSpeed(fadeSpeed), maxAlpha(maxAlpha), isFadeIn(isFadeIn), moveAngle(0)
{
	if (isFadeIn)
	{
		color.a = 0;
	}
}


Particle::~Particle()
{
}


void Particle::update(float dt)
{
	Sprite::update(dt);

	if (type == FADE)
	{
		if (isFadeIn)
		{
			color.a += fadeSpeed * dt;
			if (color.a > maxAlpha)
			{
				isFadeIn = false;
			}
		}
		else
		{
			color.a -= fadeSpeed * dt;
			if (color.a < 0)
			{
				deleting = true;
			}
		}
	}
	else if (type == BUBBLE)
	{
		moveAngle += dt;
		pos.x += cos(moveAngle) * 30 * dt;
		pos.y -= 100 * dt;

		if (pos.y < -rect.height())
		{
			deleting = true;
		}
	}
	else if (type == BUBBLE2)
	{
		scale -= Vec2(1, 1) * dt;
		pos += Vec2(cos(moveAngle), sin(moveAngle)) * 100 * dt;

		if (moveAngle < D3DXToRadian(269))
		{
			moveAngle += dt;
		}
		else if(moveAngle > D3DXToRadian(271))
		{
			moveAngle -= dt;
		}

		if (scale.x < 0)
			deleting = true;
	}
	else if (type == SMOKE)
	{
		pos += Vec2(cos(moveAngle), sin(moveAngle)) * 30 * dt;
		scale += Vec2(0.3, 0.3) * dt;

		color.a -= fadeSpeed * dt;

		if (color.a < 0)
		{ 
			deleting = true;
		}
	}
	else if (type == PAPER)
	{
		moveAngle += dt;
		pos.x += cos(moveAngle) * 30 * dt;
		pos.y += 100 * dt;

		rotation = moveAngle;

		if (pos.y > 720)
		{
			deleting = true;
		}
	}
}