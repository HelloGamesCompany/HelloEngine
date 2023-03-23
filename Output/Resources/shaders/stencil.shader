#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormals;

	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	uniform float outlineSize;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos + aNormals * outlineSize, 1.0f);
	} 
#endif
#ifdef FRAGMENT_PROGRAM
	out vec4 FragColor;

	void main()
	{
		FragColor = vec4(1, 0.5, 0, 1.0);
	}
#endif