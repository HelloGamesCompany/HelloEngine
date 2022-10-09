#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 model;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec2 texCoords;
layout (location = 4) in int texID;

uniform mat4 view;
uniform mat4 projection;

out vec2 TextureCoords;
out int TextureID;

void main()
{
    TextureCoords = texCoords;
    TextureID = texID;
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}