#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec4 oColor;

uniform float uVertexModifier = 1;
uniform vec3 uVertexOffset = vec3(0, 0, 0);

void main()
{
    gl_Position = vec4(aPosition * uVertexModifier + uVertexOffset, 1.0);
    oColor = vec4(aColor, 1);
    // oColor = gl_Position;
};

