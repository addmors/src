#version 410 core 
 
#extension GL_EXT_gpu_shader4 : enable
 
out uvec3 FragColor;
 
uniform uint gDrawIndex;
uniform uint gObjectIndex;
 
void main()
{
    FragColor = uvec3(gObjectIndex, gDrawIndex, gl_PrimitiveID+1);
}