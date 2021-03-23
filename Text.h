#pragma once
#include "Entity.h"

class Text :
	public Entity
{
public:
	Text(string text, int size, D3DXCOLOR color);
	~Text();

	void render();

	D3DXCOLOR color;
	string text;
	LPD3DXFONT font;
};

