#pragma once

#include "Texture.h"

class TextureManager
{
public:

	static float BindTexture(uint texture);

	static uint bindedTextures;
	static std::map<uint, Texture> loadedTextures;
	static std::map<std::string, uint> usedPaths;
};

