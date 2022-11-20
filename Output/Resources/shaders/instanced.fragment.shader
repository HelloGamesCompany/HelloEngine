#version 330 core
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