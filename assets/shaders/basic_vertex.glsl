#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = gl_Position + vec4(0.3, 0.3, 0.3, 1);
    vertexColor.a = 0.9;
};

