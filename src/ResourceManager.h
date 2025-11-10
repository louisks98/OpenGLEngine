//
// Created by Louis on 11/8/2025.
//

#ifndef OPENGLENGINE_RESOURCEMANAGER_H
#define OPENGLENGINE_RESOURCEMANAGER_H
#include <cstdint>

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"


class ResourceManager
{
public:
    const Mesh *GetMesh(uint32_t id) const;
    uint32_t AddMesh(Mesh&& mesh);

    const Material *GetMaterial(uint32_t id) const;
    uint32_t AddMaterial(Material&& material);

    const Shader *GetShader(uint32_t id) const;
    uint32_t AddShader(Shader&& shader);

    uint32_t GetShaderIndexByName(const std::string &name) const;
private:
    std::unordered_map<uint32_t, Mesh> meshes;
    std::unordered_map<uint32_t, Material> materials;
    std::unordered_map<uint32_t, Shader> shaders;

    uint32_t meshId = 0;
    uint32_t materialId = 0;
    uint32_t shaderId = 0;
};


#endif //OPENGLENGINE_RESOURCEMANAGER_H