#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Camera {
    vec3 position;
};

in vec2 o_TexCoords;
in vec3 o_Normal;
in vec3 o_FragPosition;

uniform Material material;
uniform Light light;
uniform Camera camera;

void main()
{
    // Ambient
    vec3 ambientLight = light.ambient * vec3(texture(material.diffuse, o_TexCoords));

    // Diffuse
    vec3 normalVector = normalize(o_Normal);
    vec3 lightDirection = normalize(light.position - o_FragPosition);
    float diffuse = max(dot(normalVector, lightDirection), 0);
    vec3 diffuseLight = light.diffuse * (diffuse * vec3(texture(material.diffuse, o_TexCoords)));

    // Specular
    vec3 cameraDirection = normalize(camera.position - o_FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normalVector);
    float specular = pow(max(dot(cameraDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specularLight = light.specular * (specular * material.specular);

    FragColor = vec4(ambientLight + diffuseLight + specularLight, 1.0f);
};

