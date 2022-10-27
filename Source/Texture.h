#pragma once
class Texture
{
public:
	Texture();
	~Texture();

	std::string name="";
	uint OpenGLID = 0;
	int width = 0;
	int height = 0;
};

