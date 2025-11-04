#version 460 core
#include "common/structures.glsl"
#include "common/lighting.glsl"

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numPointLights;
uniform int numSpotLights;
uniform vec3 viewPos;

void main()
{

    vec3 outputColor = CalculatePhongDirectionalLighting(Normal, FragPos, viewPos,
    directionalLight, material.mainColor, material.specular, material.shininess);

    for(int i = 0; i < numPointLights; i++)
    {
        outputColor += CalculatePhongPointLighting(Normal, FragPos, viewPos,
        pointLights[i], material.mainColor, material.specular, material.shininess);
    }

    for(int i = 0; i < numSpotLights; i++)
    {
        outputColor += CalculatePhongSpotLighting(Normal, FragPos, viewPos,
        spotLights[i], material.mainColor, material.specular, material.shininess);
    }

    FragColor = vec4(outputColor, 1.0);
}