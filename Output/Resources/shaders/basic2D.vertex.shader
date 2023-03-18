#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 textCoords;

uniform mat4 model;

out vec2 TextureCoords;

void main()
{
    gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    TextureCoords = textCoords;
}