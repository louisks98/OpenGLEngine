//
// Created by Louis on 11/5/2025.
//

#ifndef OPENGLENGINE_MODELIMPORTER_H
#define OPENGLENGINE_MODELIMPORTER_H
#include <string>
#include <memory>

// Forward declarations
class Entity;
class Mesh;
class Model;
class Shader;
struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

struct ShaderPool
{
    std::shared_ptr<Shader> phongShader;
    std::shared_ptr<Shader> phongMapsShader;

    ShaderPool(std::shared_ptr<Shader> phong, std::shared_ptr<Shader> phongMaps)
        : phongShader(std::move(phong)), phongMapsShader(std::move(phongMaps)) {}
};

class ModelImporter
{
public:
    static Entity Import(const std::string& path, const ShaderPool& shaders);
    static Entity Import(const std::string& path);

private:
    static Entity ImportInternal(const std::string& path, const ShaderPool* shaders = nullptr);
    static Mesh GenerateVertexData(aiMesh* assimpMesh);
    static Model GenerateModel(aiMaterial* assimpMaterial, Mesh& mesh, const std::string& directory, const ShaderPool* shaders = nullptr);
    static void GenerateSceneGraph(Entity& entity, const aiScene* scene, const aiNode* assimpNode, const std::string& directory, const ShaderPool* shaders = nullptr);
};


#endif //OPENGLENGINE_MODELIMPORTER_H