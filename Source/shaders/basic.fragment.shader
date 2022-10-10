#version 330 core
out vec4 FragColor;

in vec2 TextureCoords;
in float TextureID;

uniform sampler2D textures[32];

void main()
{
    FragColor = texture(textures[int(TextureID)], TextureCoords);
} 