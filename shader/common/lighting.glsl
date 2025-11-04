// Common lighting calculations

vec3 CalculateAmbient(vec3 diffuseColor, vec3 lightColor)
{
    float ambientStrength = 0.1;
    return diffuseColor * lightColor * ambientStrength;
}

vec3 CalculateDiffuse(vec3 normal, vec3 lightDir, vec3 lightColor, float lightIntensity, vec3 diffuseColor)
{
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (lightColor * lightIntensity) * (diff * diffuseColor);
    return diffuse;
}

vec3 CalculateSpecular(vec3 normal, vec3 viewDir, vec3 lightDir, vec3 lightColor, float lightIntensity, vec3 specularColor, float shininess)
{
    vec3 norm = normalize(normal);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = (lightColor * lightIntensity) * (spec * specularColor);
    return specular;
}


vec3 CalculatePhongPointLighting(vec3 normal, vec3 fragPos, vec3 viewPos, PointLight light,
                            vec3 diffuseColor, vec3 specularColor, float shininess)
{
    vec3 ambient = CalculateAmbient(diffuseColor, light.color);

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 diffuse = CalculateDiffuse(normal, lightDir, light.color, light.intensity, diffuseColor);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 specular = CalculateSpecular(normal, viewDir, lightDir, light.color, light.intensity, specularColor, shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return (ambient *= attenuation) + (diffuse *= attenuation) + (specular *= attenuation);
}

vec3 CalculatePhongDirectionalLighting(vec3 normal, vec3 fragPos, vec3 viewPos, DirectionalLight light,
                                    vec3 diffuseColor, vec3 specularColor, float shininess)
{
    vec3 ambient = CalculateAmbient(diffuseColor, light.color);

    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = CalculateDiffuse(normal, lightDir, light.color, light.intensity, diffuseColor);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 specular = CalculateSpecular(normal, viewDir, lightDir, light.color, light.intensity, specularColor, shininess);

    return ambient + diffuse + specular;
}

vec3 CalculatePhongSpotLighting(vec3 normal, vec3 fragPos, vec3 viewPos, SpotLight light,
                                    vec3 diffuseColor, vec3 specularColor, float shininess)
{
    vec3 ambient = CalculateAmbient(diffuseColor, light.color);
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.spotDirection));

    if(theta > light.cutoff)
    {

        vec3 lightDir = normalize(light.position - fragPos);
        vec3 diffuse = CalculateDiffuse(normal, lightDir, light.color, light.intensity, diffuseColor);

        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 specular = CalculateSpecular(normal, viewDir, lightDir, light.color, light.intensity, specularColor, shininess);

        return ambient + diffuse + specular;
    }

    return ambient;
}
