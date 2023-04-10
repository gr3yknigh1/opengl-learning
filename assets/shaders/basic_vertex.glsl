#version 330 core

layout (location = 0) in vec3 l_Position;
layout (location = 1) in vec2 l_TexCoord;

out vec2 f_TexCoord;

uniform mat4 u_Transform;

void main()
{
    gl_Position = u_Transform * vec4(l_Position, 1.0);
    f_TexCoord = l_TexCoord;
};

