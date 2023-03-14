#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in mat4 model;
	layout (location = 5) in vec3 normals;
	layout (location = 6) in vec2 texCoords;
	layout (location = 7) in float texID;

	out vec2 TextureCoords;
	out float TextureID;

	void main()
	{
		gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
		TextureCoords = texCoords;
		TextureID = texID;
	}
#endif
#ifdef FRAGMENT_PROGRAM
	out vec4 FragColor;

	in vec2 TextureCoords;
	in float TextureID;

	uniform sampler2D textures[32];

	void main()
	{
		const float eps = 0.001;
		if (abs(TextureID + 1.0) < eps)
		{
			FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
		}
		else
		{
			FragColor = texture(textures[int(round(TextureID))], TextureCoords);
		}
	}
#endif