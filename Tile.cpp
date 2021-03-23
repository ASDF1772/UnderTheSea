#include "DXUT.h"
#include "Tile.h"
#include "GameManager.h"

Tile::Tile(TileType type, TeamColor teamColor) 
	:type(type), ship(nullptr), is_limit(false), is_smoke(false), moveAngle(0), smokeTimer(0)
{
	spr = new Sprite("image/Game/Tile/" + to_string(random(0, 1)) + "/" + to_string((bool)random(0, 20)) + ".png");
	filter = new Sprite("image/Game/Tile/limit.png");
	mark = new Sprite("image/Game/Unit/mark.png");
	debris = new Sprite("image/Game/Unit/Debris/" + to_string(teamColor) + ".png");

	addChild(spr);
	addChild(filter);
	addChild(mark);
	addChild(debris);

	debris->pos = Vec2(0, -10);
	mark->pos = Vec2(15, -20);

	mark->visible = false;
	debris->visible = false;
	filter->visible = false;
	rect = spr->rect;
}


Tile::~Tile()
{
}


void Tile::update(float dt)
{
	Entity::update(dt);

	if (mark->visible)
	{
		moveAngle += 1.5 * dt;
		mark->pos.y += cos(moveAngle)* 10 * dt;
	}
	if (is_smoke)
	{
		smokeTimer += dt;

		if (smokeTimer > 0.2)
		{
			Particle* p = new Particle("image/Game/Effect/smoke.png", SMOKE, 0.3);
			gm.ingame->addChild(p);
			gm.ingame->particleList.push_back(p);
			p->setCenter(center());
			p->scaleCenter = p->rect.center();
			p->scale = Vec2(0.2, 0.2);
			p->color.a = 0.8;
			p->moveAngle = D3DXToRadian(random(260, 280));
			smokeTimer = 0;
		}
	}
}


void Tile::setLimit()
{
	//type = LIMIT_TILE;
	filter->setPath("image/Game/Tile/limit.png");
	filter->visible = true;
	pos.y += 3;
	is_limit = true;
}


Vec2 Tile::center()
{
	return parent->pos + pos + Vec2(TILE_WIDTH, TILE_HEIGHT) / 2;
}