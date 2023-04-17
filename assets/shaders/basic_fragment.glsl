#version 330 core

out vec4 FragColor;

in vec2 f_TexCoord;

uniform vec3 u_Color;
uniform sampler2D u_Texture0;

void main()
{
    FragColor = texture(u_Texture0, f_TexCoord) * vec4(u_Color, 1);
};

