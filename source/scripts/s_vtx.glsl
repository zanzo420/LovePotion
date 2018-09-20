#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec4 vtxColor;
//out vec2 vtxTexCoord;

uniform mat4 transMtx;

void main()
{
    // Set up position and color
    gl_Position = transMtx * vec4(inPos, 1.0);
    vtxColor = inColor;

    // Calculate texcoord
    //vtxTexCoord = inTexCoord;
}