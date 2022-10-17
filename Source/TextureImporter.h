#pragma once
class TextureImporter
{
public:
	//TODO: The system to check whether an image has already been loaded or not should be inside the ResourceManager, not here.

	// Loads an image file and saves it as DDS inside Library.
	static void ImportImage(const char* fileName, char* buffer, int size); 

	// Loads a texture from DDS file to OpenGL and returns its ID.
	static uint Load(char* buffer, int size, int* w, int* h);

	static uint ImportTexture(std::string path);
	static uint CheckerImage();
	static uint ImportTextureSTBI(std::string path);
};

