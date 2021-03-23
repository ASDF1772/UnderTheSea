#include "DXUT.h"
#include "DeployTile.h"


DeployTile::DeployTile()
{
	spr = new Sprite("image/Deploy/Tile/" + to_string(random(0, 1)) + "/" + to_string((bool)random(0, 20)) + ".png");
	filter = new Sprite("image/Deploy/Tile/limit.png");

	addChild(spr);
	addChild(filter);

	filter->visible = false;
	rect = spr->rect;
}


DeployTile::~DeployTile()
{
}