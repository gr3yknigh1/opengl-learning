#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

out vec4 FragColor;

in vec3 o_Normal;
in vec3 o_FragPosition;

uniform Material material;

uniform vec3 u_AmbientColor;
uniform vec3 u_Color;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

void main()
{
    // Ambient
    vec3 ambientLight = u_AmbientColor * material.ambient;

    // Diffuse
    vec3 normalVector = normalize(o_Normal);
    vec3 lightDirection = normalize(u_LightPosition - o_FragPosition);
    float diffuse = max(dot(normalVector, lightDirection), 0);
    vec3 diffuseLight = u_LightColor * (diffuse * material.diffuse);

    // Specular
    vec3 cameraDirection = normalize(u_CameraPosition - o_FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normalVector);
    float specular = pow(max(dot(cameraDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specularLight = u_LightColor * (specular * material.specular);

    FragColor = vec4(ambientLight + diffuseLight + specularLight, 1.0f);
};

