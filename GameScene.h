#pragma once
#include "Scene.h"
#include "StageMap.h"
#include "Animation.h"
#include "Particle.h"
#include "Bullet.h"
#include "UI.h"

class GameScene :
	public Scene
{
public:
	GameScene();
	~GameScene();

	void render();
	void update(float dt);
	void setFilter(Tile* tile, bool visible);
	void spawnBullet(BulletType type, Team team, Tile* targetTile, bool is_main = true, bool is_sub = false);

	Sprite* bg;
	UI* ui;
	Sprite* pointer;

	StageMap* playerMap;
	StageMap* enemyMap;

	Tile* pFocusTile;
	Tile* eFocusTile;

	bool is_player_turn;
	bool is_enemy_turn;

	bool is_player_attack;
	bool is_enemy_attack;

	bool is_player_turn_end;
	bool is_enemy_turn_end;

	float turnTimer;

	float moveAngle;

	float fishTimer;
	float sceneTimer;
	float bubblePos;
	float bubbleTimer;
	Timer spawnTimer;

	Timer pBarrageTimer;
	Timer eBarrageTimer;

	pair<float, float>thinkTimer;

	vector<Bullet*> bulletList;
	vector<Animation*> effectList;
	vector<Particle*> particleList;
	vector<Animation*>fishList;

	Vec2 lastMousePos;
};