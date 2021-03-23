#include "DXUT.h"
#include "Animation.h"
#include "Asset.h"


Animation::Animation(const string& path, int frameNum, float fps, bool loop, bool png)
	:currentFrame(0), fps(fps), loop(loop), is_end(false)
{
	for (int i = 0; i < frameNum; i++)
	{
		textures.push_back(asset.getTexture(path + to_string(i) + (png ? ".png" : ".dds")));
	}
	texture = textures[0];

	rect = visibleRect = Rect(0, 0, texture->info.Width, texture->info.Height);
}


Animation::~Animation()
{
}


void Animation::update(float dt)
{
	Sprite::update(dt);

	currentFrame += fps * dt;

	if ((int)currentFrame >= textures.size())
	{
		if (loop)
		{
			currentFrame = 0;
		}
		else
		{
			is_end = true;
			currentFrame = textures.size() - 1;
		}
	}

	texture = textures[(int)currentFrame];
}