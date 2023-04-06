#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec4 fColor;
uniform float mod = 1;

void main()
{
    gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
    fColor = vec4(aColor, 1) * mod;
};

