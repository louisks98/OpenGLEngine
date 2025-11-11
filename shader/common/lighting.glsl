// Common lighting calculations

vec4 CalculateAmbient(vec4 diffuseColor, vec3 lightColor)
{
    float ambientStrength = 0.1;
    return vec4 (diffuseColor.rgb * lightColor * ambientStrength, diffuseColor.a);
}

vec4 CalculateDiffuse(vec3 normal, vec3 lightDir, vec3 lightColor, float lightIntensity, vec4 diffuseColor)
{
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = vec4((lightColor * lightIntensity) * (diff * diffuseColor.rgb), diffuseColor.a);
    return diffuse;
}

vec4 CalculateSpecular(vec3 normal, vec3 viewDir, vec3 lightDir, vec3 lightColor, float lightIntensity, vec4 specularColor, float shininess)
{
    vec3 norm = normalize(normal);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specular = vec4((lightColor * lightIntensity) * (spec * specularColor.rgb), specularColor.a);
    return specular;
}


vec4 CalculatePhongPointLighting(vec3 normal, vec3 fragPos, vec3 viewPos, PointLight light,
                            vec4 diffuseColor, vec4 specularColor, float shininess)
{
    vec4 ambient = CalculateAmbient(diffuseColor, light.color);

    vec3 lightDir = normalize(light.position - fragPos);
    vec4 diffuse = CalculateDiffuse(normal, lightDir, light.color, light.intensity, diffuseColor);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec4 specular = CalculateSpecular(normal, viewDir, lightDir, light.color, light.intensity, specularColor, shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return (ambient *= attenuation) + (diffuse *= attenuation) + (specular *= attenuation);
}

vec4 CalculatePhongDirectionalLighting(vec3 normal, vec3 fragPos, vec3 viewPos, DirectionalLight light,
                                    vec4 diffuseColor, vec4 specularColor, float shininess)
{
    vec4 ambient = CalculateAmbient(diffuseColor, light.color);

    vec3 lightDir = normalize(-light.direction);
    vec4 diffuse = CalculateDiffuse(normal, lightDir, light.color, light.intensity, diffuseColor);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec4 specular = CalculateSpecular(normal, viewDir, lightDir, light.color, light.intensity, specularColor, shininess);

    return ambient + diffuse + specular;
}

vec4 CalculatePhongSpotLighting(vec3 normal, vec3 fragPos, vec3 viewPos, SpotLight light,
                                    vec4 diffuseColor, vec4 specularColor, float shininess)
{
    vec4 ambient = CalculateAmbient(diffuseColor, light.color);
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.spotDirection));

    if(theta > light.cutoff)
    {

        vec3 lightDir = normalize(light.position - fragPos);
        vec4 diffuse = CalculateDiffuse(normal, lightDir, light.color, light.intensity, diffuseColor);

        vec3 viewDir = normalize(viewPos - fragPos);
        vec4 specular = CalculateSpecular(normal, viewDir, lightDir, light.color, light.intensity, specularColor, shininess);

        return ambient + diffuse + specular;
    }

    return ambient;
}
