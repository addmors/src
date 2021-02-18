#version 410 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = model* vec4(aPos.x, aPos.y, -190, 1.0); 
}  