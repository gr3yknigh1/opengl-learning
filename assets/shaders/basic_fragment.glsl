#version 330 core

out vec4 FragColor;

in vec3 o_Normal;
in vec3 o_FragPosition;

uniform float u_AmbientStr;
uniform vec3 u_Color;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;
uniform float u_SpecularStr;
uniform int u_SpecularShininess;

void main()
{
    // Ambient
    vec3 ambientLight = u_AmbientStr * u_LightColor;

    // Diffuse
    vec3 normalVector = normalize(o_Normal);
    vec3 lightDirection = normalize(u_LightPosition - o_FragPosition);
    float diffuse = max(dot(normalVector, lightDirection), 0);
    vec3 diffuseLight = diffuse * u_LightColor;

    // Specular
    vec3 cameraDirection = normalize(u_CameraPosition - o_FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normalVector);
    float specular = pow(max(dot(cameraDirection, reflectionDirection), 0), u_SpecularShininess);
    vec3 specularLight = u_SpecularStr * specular * u_LightColor;

    FragColor = vec4((ambientLight + diffuseLight + specularLight) * u_Color, 1.0f);
};

