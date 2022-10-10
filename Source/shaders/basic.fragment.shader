#version 330 core
out vec4 FragColor;

in vec2 TextureCoords;
in int TextureID;

uniform sampler2D testTexture;

void main()
{
    FragColor = texture(testTexture, TextureCoords);
    //FragColor = vec4(TextureCoords.x, TextureCoords.y, 0.0f, 1.0f);
} 