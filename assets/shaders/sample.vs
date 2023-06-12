#version 330 core

layout (location = 0) in vec3 m_Position;

void main()
{
    gl_Position = vec4(m_Position, 1.0f);
};

// vim: set filetype=glsl:
