#version 330 core

out vec4 FragColor;

in vec4 f_Color;
in vec2 f_TexCoord;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

void main()
{
    FragColor = mix(texture(u_Texture0, f_TexCoord) * f_Color,
                    texture(u_Texture1, f_TexCoord), 0.2);
};

