#version 330 core

out vec4 FragColor;

in vec2 o_TexCoords;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, o_TexCoords);
};

// vim: set filetype=glsl:
