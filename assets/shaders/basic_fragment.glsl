#version 330 core

out vec4 FragColor;

uniform float u_AmbientStr;
uniform vec3 u_Color;
uniform vec3 u_LightColor;

void main()
{
    vec3 ambientLight = u_AmbientStr * u_LightColor;
    FragColor = vec4(u_Color * ambientLight, 1.0f);
};

