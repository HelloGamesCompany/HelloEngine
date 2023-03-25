#version 410 core
#ifdef VERTEX_PROGRAM
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}  
#endif
#ifdef FRAGMENT_PROGRAM
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}  
#endif