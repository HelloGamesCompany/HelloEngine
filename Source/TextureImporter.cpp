#include "Headers.h"
#include "TextureImporter.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "Texture.h"
#include "TextureManager.h"
#include "External/stb_image/stb_image.h"


void TextureImporter::ImportImage(const char* fileName, char* buffer, int size)
{
	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	if (!ilLoadImage(fileName))
	{
		LOG("Error loading image: %s", ilutGetString(ilGetError()));
	}

	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer

	if (size > 0) 
	{
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
			buffer = (char*)data;

		RELEASE_ARRAY(data);
	}

	ilDeleteImages(1, &ImgId);
}

uint TextureImporter::Load(char* buffer, int size, int* width, int* heigth)
{
	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		LOG("Error loading image: %s", ilutGetString(ilGetError()));
	}

	Texture engineTexture;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	engineTexture.OpenGLID = ilutGLBindTexImage();

	//TODO: Generate mipmaps and use best settings
	glBindTexture(GL_TEXTURE_2D, engineTexture.OpenGLID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	*width = ilGetInteger(IL_IMAGE_WIDTH);
	*heigth = ilGetInteger(IL_IMAGE_HEIGHT);

	TextureManager::loadedTextures[engineTexture.OpenGLID] = engineTexture; // Add loaded texture inside TextureManager.
}

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
	if (!ilLoadImage(path.c_str()))
	{
		LOG("Error loading image: %s", ilutGetString(ilGetError()));
	}

	GLuint texture = ilutGLBindTexImage();

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &ImgId);

	Texture engineTexture;
	engineTexture.OpenGLID = texture;
	engineTexture.name = path;

	TextureManager::loadedTextures[texture] = engineTexture; // Add loaded texture inside TextureManager.
	TextureManager::usedPaths[path] = texture;

	return texture;
}

uint TextureImporter::CheckerImage()
{
	GLubyte checkerImage[240][240][4];
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 240; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	uint textureID = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 240, 240,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	return textureID;
}

uint TextureImporter::ImportTextureSTBI(std::string path)
{
	//Check if the given texture has been already loaded
	if (TextureManager::usedPaths.find(path) != TextureManager::usedPaths.end())
	{
		return TextureManager::usedPaths[path]; // If this texture path was already loaded, return the loaded texture.
	}

	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;	// BPP = Bits per pixel

	//stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}

	Texture engineTexture;
	engineTexture.OpenGLID = m_RendererID;
	engineTexture.name = path;

	TextureManager::loadedTextures[m_RendererID] = engineTexture; // Add loaded texture inside TextureManager.
	TextureManager::usedPaths[path] = m_RendererID;

	return m_RendererID;
}
