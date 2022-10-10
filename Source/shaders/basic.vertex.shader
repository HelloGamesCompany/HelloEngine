#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 model;
layout (location = 5) in vec3 normals;
layout (location = 6) in vec2 texCoords;
layout (location = 7) in int texID;

uniform mat4 view;
uniform mat4 projection;

out vec2 TextureCoords;
out int TextureID;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    TextureCoords = texCoords;
    TextureID = texID;
}