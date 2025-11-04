// Common shader structures

struct Material
{
    vec3 mainColor;
    vec3 specular;
    float shininess;
};

struct MaterialMaps
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight
{
    vec3 position;
    vec3 color;
    float intensity;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 spotDirection;
    vec3 color;
    float intensity;
    float cutoff;
};
