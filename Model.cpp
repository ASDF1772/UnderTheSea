#include "DXUT.h"
#include "Model.h"


Model::Model(UnitType type, TeamColor teamColor, int size, int index) 
	:type(type), size(size), is_deployed(false), is_go_origin(false), is_auto_set(false), originRotation(0)
{
	cell = new Cell[size];

	for (int i = 0; i < size; i++)
	{
		cell[i].x = i;
		cell[i].y = 0;
	}


	if (type == SHIP)
	{
		if (size != 4)
		{
			rotation = originRotation = D3DXToRadian(90);
			setCell();
		}
		//spr = new Sprite("image/Deploy/Unit/1/" + to_string(index) + ".png");
		spr = new Sprite("image/Deploy/Unit/"+to_string(teamColor)+"/" + to_string(index) + ".png");
		offset = Vec2(0, 20);
	}
	else if (type == OBJECT)
	{
		spr = new Sprite("image/Deploy/Unit/Object/" + to_string(size) + ".png");
		offset = Vec2(0, 0);
	}
	else if (type == MINE)
	{
		spr = new Sprite("image/Deploy/Unit/mine.png");

		offset = Vec2(0, 0);
	}

	addChild(spr);

	rotationCenter = offset + Vec2(0.5, 0.5) * DEPLOY_SIZE;
	rect = spr->rect;
}


Model::~Model()
{
}


void Model::update(float dt)
{
	Entity::update(dt);

	if (is_go_origin)
	{
		pos += (originPos - pos) * 10 * dt;
		if (distance(pos, originPos) < 2)
		{
			is_go_origin = false;
			pos = originPos;
		}
	}
	if (is_auto_set)
	{
		pos += (movePos - pos) * 10 * dt;
		if (distance(pos, movePos) < 2)
		{
			is_auto_set = false;
			is_deployed = true;
			pos = movePos;
		}
	}
}


void Model::setCell()
{
	int x = (int)cos(rotation);
	int y = (int)sin(rotation);

	for (int i = 0; i < size; i++)
	{
		cell[i].x = x * i;
		cell[i].y = y * i;
	}
}


bool Model::contain(Vec2 pos)
{
	pos -= parent->pos;

	Rect r = Rect(0, 0, DEPLOY_SIZE, DEPLOY_SIZE);

	int i = size - 1;
	if (cell[i].x < 0)
	{
		r.left -= i * DEPLOY_SIZE;
	}
	else if (cell[i].x > 0)
	{
		r.right += i * DEPLOY_SIZE;
	}

	if (cell[i].y < 0)
	{
		r.top -= i * DEPLOY_SIZE;
	}
	else if(cell[i].y > 0)
	{
		r.bottom += i * DEPLOY_SIZE;
	}

	return r.offset(offsetPos()).contain(pos);
}


Vec2 Model::offsetPos()
{
	return pos + offset;
}


Vec2 Model::offsetCenter()
{
	return pos + offset + Vec2(0.5, 0.5) * DEPLOY_SIZE;
}