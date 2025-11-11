// Common shader structures

const uint Opaque = 0;
const uint Translucent = 1;

struct Material
{
    vec4 mainColor;
    vec4 specular;
    float shininess;
    uint type;
};

struct MaterialMaps
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    uint type;
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
