//
// Created by Louis on 11/8/2025.
//

#include "ResourceManager.h"

uint32_t ResourceManager::AddMesh(Mesh &&mesh)
{
    meshId++;
    meshes.insert({meshId, std::move(mesh)});
    return meshId;
}

const Mesh *ResourceManager::GetMesh(const uint32_t id) const
{
    if (const auto it = meshes.find(id); it != meshes.end())
        return &it->second;

    return nullptr;
}

uint32_t ResourceManager::AddMaterial(Material &&material)
{
    materialId++;
    materials.insert({materialId, std::move(material)});
    return materialId;
}

const Material *ResourceManager::GetMaterial(const uint32_t id) const
{
    if (const auto it = materials.find(id); it != materials.end())
        return &it->second;

    return nullptr;
}

uint32_t ResourceManager::AddShader(Shader &&shader)
{
    shaderId++;
    shaders.insert({shaderId, std::move(shader)});
    return shaderId;
}

uint32_t ResourceManager::GetShaderIndexByName(const std::string &name) const
{
    for (auto it = shaders.begin(); it != shaders.end(); ++it)
    {
        if (it->second.GetShaderName() == name)
            return it->first;
    }

    return -1;
}

const Shader *ResourceManager::GetShader(const uint32_t id) const
{
    if (const auto it = shaders.find(id); it != shaders.end())
        return &it->second;

    return nullptr;
}





