#version 460 core
#include "common/structures.glsl"
#include "common/lighting.glsl"

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform MaterialMaps material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numPointLights;
uniform int numSpotLights;

uniform vec3 viewPos;

void main()
{
    vec4 diffTexSampling = texture(material.diffuse, texCoord);
    vec4 specTexSampling = texture(material.specular, texCoord);

    vec4 outputColor = CalculatePhongDirectionalLighting(Normal, FragPos, viewPos,
    directionalLight, diffTexSampling, specTexSampling, material.shininess);

    for(int i = 0; i < numPointLights; i++)
    {
        vec4 color = CalculatePhongPointLighting(Normal, FragPos, viewPos,
        pointLights[i], diffTexSampling, specTexSampling, material.shininess);
        outputColor.rgb += color.rgb;
    }

    for(int i = 0; i < numSpotLights; i++)
    {
        vec4 color = CalculatePhongSpotLighting(Normal, FragPos, viewPos,
        spotLights[i], diffTexSampling, specTexSampling, material.shininess);
        outputColor.rgb += color.rgb;
    }

    if(material.type == Translucent)
    {
        if(outputColor.a < 0.01)
        discard;
    }

    FragColor = outputColor;
}