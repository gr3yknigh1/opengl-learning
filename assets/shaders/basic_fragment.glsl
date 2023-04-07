#version 330 core

out vec4 FragColor;

in vec4 f_Color;
in vec2 f_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, f_TexCoord);
};

