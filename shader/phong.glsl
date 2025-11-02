#version 460 core
struct Material
{
    vec3 mainColor;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 color;
    float intensity;
};

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = material.mainColor * light.color * ambientStrength;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (light.color * light.intensity) * (diff * material.mainColor);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0) , material.shininess);
    vec3 specular = (light.color * light.intensity) * (spec * material.specular);

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}