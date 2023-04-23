#version 330 core

layout (location = 0) in vec3 l_Position;
layout (location = 1) in vec3 l_Normal;
layout (location = 2) in vec2 l_TexCoords;

out vec2 o_TexCoords;
out vec3 o_Normal;
out vec3 o_FragPosition;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(l_Position, 1.0);
    o_FragPosition = vec3(u_Model * vec4(l_Position, 1.0));
    o_Normal = mat3(transpose(inverse(u_Model))) * l_Normal;
    //         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ - constly operation
    // NOTE: Better perform this calculation in CPU

    o_TexCoords = l_TexCoords;
};

