//
// Created by Louis on 11/8/2025.
//

#ifndef OPENGLENGINE_RESOURCEMANAGER_H
#define OPENGLENGINE_RESOURCEMANAGER_H
#include <cstdint>

#include "../rendering/Material.h"
#include "../rendering/Mesh.h"
#include "../rendering/Shader.h"


class ResourceManager
{
public:
    const Mesh *GetMesh(uint32_t id) const;
    uint32_t AddMesh(Mesh&& mesh);

    const Material *GetMaterial(uint32_t id) const;
    uint32_t AddMaterial(Material&& material);
    uint32_t AddMaterial(uint32_t shaderId, glm::vec4 mainColor, glm::vec4 specular, float shininess);
    uint32_t AddTranslucentMaterial(uint32_t shaderId, glm::vec4 mainColor, glm::vec4 specular, float shininess);
    uint32_t AddTextureMaterial(uint32_t shaderId, const std::shared_ptr<Texture> &diffuse, const std::shared_ptr<Texture> &specular, float shininess);
    uint32_t AddTranslucentTextureMaterial(uint32_t shaderId, const std::shared_ptr<Texture> &diffuse, const std::shared_ptr<Texture> &specular, const float shininess);

    const Shader *GetShader(uint32_t id) const;
    uint32_t AddShader(Shader&& shader);
    uint32_t AddShader(const std::string &vertPath, const std::string &fragPath, const std::string &name);

    uint32_t GetShaderIndexByName(const std::string &name) const;

    uint32_t depthBufferDebugShaderId = 0;
private:
    std::unordered_map<uint32_t, Mesh> meshes;
    std::unordered_map<uint32_t, Material> materials;
    std::unordered_map<uint32_t, Shader> shaders;
    mutable std::unordered_map<std::string, uint32_t> shaderNameCache;

    uint32_t meshIdCounter = 0;
    uint32_t materialIdCounter = 0;
    uint32_t shaderIdCounter = 0;
};


#endif //OPENGLENGINE_RESOURCEMANAGER_H