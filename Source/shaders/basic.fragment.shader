#version 330 core
out vec4 FragColor;

in vec2 TextureCoords;
in int TextureID;

uniform sampler2D textures[32];
uniform sampler2D testTexture;

void main()
{
    FragColor = texture(testTexture, TextureCoords);

    //FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  /*  if (TextureID == -1)
    {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else
    {
        FragColor = texture(textures[TextureID], TextureCoords);
    }*/
} 