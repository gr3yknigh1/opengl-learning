#version 330 core

layout (location = 0) in vec3 m_Position;
layout (location = 1) in vec3 m_Color;
layout (location = 2) in vec2 m_TexCoords;

out vec2 o_TexCoords;

void main()
{
    gl_Position = vec4(m_Position, 1.0f);
    o_TexCoords = m_TexCoords;
};

// vim: set filetype=glsl:
