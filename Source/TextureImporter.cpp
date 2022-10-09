#include "Headers.h"
#include "TextureImporter.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "Texture.h"
#include "TextureManager.h"

uint TextureImporter::ImportTexture(std::string path)
{
	//Check if the given texture has been already loaded
	if (TextureManager::usedPaths.find(path) != TextureManager::usedPaths.end())
	{
		return TextureManager::usedPaths[path]; // If this texture path was already loaded, return the loaded texture.
	}

	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	ilLoadImage(path.c_str());
	ilBindImage(ImgId);

	GLuint texture;
	texture = ilutGLBindTexImage();

	Texture engineTexture;
	engineTexture.OpenGLID = texture;
	engineTexture.name = path;

	TextureManager::loadedTextures[texture] = engineTexture; // Add loaded texture inside TextureManager.
	TextureManager::usedPaths[path] = texture;

	return texture;
}
