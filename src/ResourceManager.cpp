//
// Created by Louis on 11/8/2025.
//

#include "ResourceManager.h"

uint32_t ResourceManager::AddMesh(Mesh &&mesh)
{
    meshIdCounter++;
    meshes.insert({meshIdCounter, std::move(mesh)});
    return meshIdCounter;
}

const Mesh *ResourceManager::GetMesh(const uint32_t id) const
{
    if (const auto it = meshes.find(id); it != meshes.end())
        return &it->second;

    return nullptr;
}

uint32_t ResourceManager::AddMaterial(Material &&material)
{
    materialIdCounter++;
    materials.insert({materialIdCounter, std::move(material)});
    return materialIdCounter;
}

uint32_t ResourceManager::AddMaterial(const uint32_t shaderId, const glm::vec4 mainColor, const glm::vec4 specular, const float shininess)
{
    auto mat = Material();
    mat.SetShader(shaderId);
    mat.SetColorProperty("material.mainColor", mainColor);
    mat.SetColorProperty("material.specular", glm::vec4(0.633f, 0.727811f, 0.633f, 0.2f));
    mat.SetFloatProperty("material.shininess", 76.8f);
    const auto matId = AddMaterial(std::move(mat));
    return matId;
}

uint32_t ResourceManager::AddTranslucentMaterial(const uint32_t shaderId, const glm::vec4 mainColor, const glm::vec4 specular, const float shininess)
{
    const auto matId = AddMaterial(shaderId, mainColor, specular, shininess);
    const auto mat = &materials[matId];
    mat->SetIntProperty("material.type", MaterialType::Translucent);
    return matId;
}

uint32_t ResourceManager::AddTextureMaterial(const uint32_t shaderId, const std::shared_ptr<Texture> &diffuse, const std::shared_ptr<Texture> &specular, const float shininess)
{
    auto mat = Material();
    mat.SetShader(shaderId);
    mat.SetTextureProperty("material.diffuse", diffuse);
    mat.SetTextureProperty("material.specular", specular);
    mat.SetFloatProperty("material.shininess", shininess);
    const auto matId = AddMaterial(std::move(mat));
    return matId;
}

uint32_t ResourceManager::AddTranslucentTextureMaterial(uint32_t shaderId, const std::shared_ptr<Texture> &diffuse, const std::shared_ptr<Texture> &specular, const float shininess)
{
    const auto matId = AddTextureMaterial(shaderId, diffuse, specular, shininess);
    const auto mat = &materials[matId];
    mat->SetIntProperty("material.type", MaterialType::Translucent);
    return matId;
}

const Material *ResourceManager::GetMaterial(const uint32_t id) const
{
    if (const auto it = materials.find(id); it != materials.end())
        return &it->second;

    return nullptr;
}

uint32_t ResourceManager::AddShader(Shader &&shader)
{
    shaderIdCounter++;
    shaders.insert({shaderIdCounter, std::move(shader)});
    return shaderIdCounter;
}

uint32_t ResourceManager::AddShader(const std::string &vertPath, const std::string &fragPath, const std::string &name)
{
    auto shader = Shader(vertPath, fragPath, name);
    return AddShader(std::move(shader));
}

uint32_t ResourceManager::GetShaderIndexByName(const std::string &name) const
{
    auto cache = shaderNameCache.find(name);
    if (cache != shaderNameCache.end())
        return cache->second;

    for (auto it = shaders.begin(); it != shaders.end(); ++it)
    {
        if (it->second.GetShaderName() == name)
        {
            shaderNameCache[name] = it->first;
            return it->first;
        }
    }

    return INT32_MAX;
}

const Shader *ResourceManager::GetShader(const uint32_t id) const
{
    if (const auto it = shaders.find(id); it != shaders.end())
        return &it->second;

    return nullptr;
}





