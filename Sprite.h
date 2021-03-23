#pragma once
#include "Entity.h"
#include "Texture.h"

class Sprite :
	public Entity
{
public:
	Sprite();
	Sprite(string path);
	virtual ~Sprite();
	void setPath(string path);
	void render();

	D3DXCOLOR color;
	Texture* texture;
};