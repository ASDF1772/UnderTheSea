#pragma once
#include "Scene.h"
#include "Animation.h"
#include "Particle.h"

class EndingScene :
	public Scene
{
public:
	EndingScene(TeamColor teamColor, bool is_win);
	~EndingScene();

	void update(float dt);

	Animation* ani;
	Particle* win;
	
	bool is_win;
};