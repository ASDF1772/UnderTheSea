#pragma once
#include "Sprite.h"

class Particle :
	public Sprite
{
public:
	Particle(string path, ParticleType type, float fadeSpeed = 3.f, float maxAlpha = 1.f, bool isFadeIn = false);
	~Particle();

	void update(float dt);

	ParticleType type;
	float fadeSpeed;
	float maxAlpha;
	bool isFadeIn;

	float moveAngle;
};

