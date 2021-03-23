#include "DXUT.h"
#include "Entity.h"


Entity::Entity()
	:pos(0, 0),
	rect(0,0,0,0),
	visibleRect(0,0,0,0),
	scale(1,1),
	scaleCenter(0,0),
	rotation(0),
	rotationCenter(0,0),
	updateEnabled(true),
	renderChildrenEnabled(true),
	visible(true),
	removing(false),
	deleting(false),
	parent(nullptr)
{
}


Entity::~Entity()
{
	for (auto child : children)
		delete child;
}


void Entity::addChild(Entity* child)
{
	child->parent = this;
	children.push_back(child);
}


void Entity::removeChild(Entity* child)
{
	child->removing = true;
}


void Entity::update(float dt)
{
	if (!updateEnabled) return;

	for (auto iter = children.begin(); iter != children.end(); iter++)
	{
		if ((*iter)->removing)
		{
			SAFE_DELETE(*iter);

			iter = children.erase(iter);

			if (iter == children.end())
				break;
		}
	}

	for (auto child : children)
		child->update(dt);
}


void Entity::render()
{
	if (!visible) return;

	D3DXMatrixTransformation2D(&matrix, &scaleCenter, 0, &scale, &rotationCenter, rotation, &pos);

	if (parent)
		matrix *= parent->matrix;

	if (!renderChildrenEnabled) return;

	for (auto child : children)
		child->render();
}


void Entity::setCenter(Vec2 pos)
{
	this->pos = pos - rect.center();
}


Vec2 Entity::center()
{
	return pos + rect.center();
}


Rect Entity::rectWithPos()
{
	return rect.offset(pos);
}