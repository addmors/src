#version 410 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 texCoord;

out vec3 Colors;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection*model*vec4(aPos.x, aPos.y, -190, 1); 
    TexCoord = texCoord;
}  