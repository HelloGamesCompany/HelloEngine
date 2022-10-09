#include "Headers.h"
#include "TextureManager.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "TextureImporter.h"

std::map<uint, Texture> TextureManager::loadedTextures;
std::map<std::string, uint> TextureManager::usedPaths;
uint TextureManager::bindedTextures = 0;

float TextureManager::BindTexture(uint texture)
{
	if (GL_TEXTURE0 + bindedTextures == GL_TEXTURE31) // If we loaded 32 textures already
		return -1;

	glBindTexture(GL_TEXTURE0 + bindedTextures, texture);

	return GL_TEXTURE0 + bindedTextures++;
}
