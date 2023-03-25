#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos;

	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	}
#endif
#ifdef FRAGMENT_PROGRAM
	out vec4 FragColor;

	uniform vec4 lineColor;

	void main()
	{
		FragColor = lineColor;
	} 
#endif