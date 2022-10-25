#pragma once
class Texture
{
public:
	Texture();
	~Texture();

	std::string name;
	uint OpenGLID;
	int width;
	int height;
};

