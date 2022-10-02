#pragma once
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();

	void SetDimensions(int width, int height);
	void SetBufferInfo();

	uint GetTexture() { return textureColorbuffer; }

private:

	void RegenerateBuffer();
private:
	uint FBO = 0; // Frame Buffer Object (contains the result of the entire frame texture)
	uint textureColorbuffer = 0;
	uint RBO = 0;

	int width;
	int height;
};

