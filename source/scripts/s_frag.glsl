#version 330 core

in vec4 vtxColor;
in vec2 vtxTexCoord;

out vec4 outColor;
uniform sampler2D image;

uniform bool hasTexture = false;

void main()
{
    if (hasTexture)
        outColor = vtxColor * texture(image, vtxTexCoord);
    else
        outColor = vtxColor;
}