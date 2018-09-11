#version 330 core
precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 ourColor;
out vec2 vtxTexCoord;

uniform mat4 transMtx;

void main()
{
    // Set up position and color
    gl_Position = transMtx * vec4(aPos, 1.0);
    outColor = aColor;

    // Calculate texcoord
    vtxTexCoord = inTexCoord
}