#version 460 core
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
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
in vec2 texCoord;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    vec3 diffTexSampling = texture(material.diffuse, texCoord).rgb;
    vec3 specTexSampling = texture(material.specular, texCoord).rgb;

    float ambientStrength = 0.1;
    vec3 ambient = diffTexSampling * light.color * ambientStrength;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (light.color * light.intensity) * (diff * diffTexSampling);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0) , material.shininess);
    vec3 specular = (light.color * light.intensity) * (spec * specTexSampling);

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}