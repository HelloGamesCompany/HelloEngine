#version 330 core
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