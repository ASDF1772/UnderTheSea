#pragma once
#include "Define.h"
#include "Texture.h"

class Asset
{
public:
	Asset();
	~Asset();

	void init();
	void dispose();
	void ready(string rootFolderPath);
	void loadNext();
	Texture* getTexture(string p);
	CSound* getSound(wstring p);

	map<string, Texture*>textures;
	map<wstring, CSound*> sounds;

	CSoundManager _s;

	vector<string> paths;
	vector<string>::iterator pathIter;

	int fileToLoad;
	int fileLoaded;
};

extern Asset asset;