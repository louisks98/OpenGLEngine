//
// Created by Louis on 11/5/2025.
//

#ifndef OPENGLENGINE_MODELIMPORTER_H
#define OPENGLENGINE_MODELIMPORTER_H
#include <string>

#include "ResourceManager.h"
#include "Scene.h"

// Forward declarations
class Entity;
class Mesh;
class Model;
class Shader;
struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;


class ModelImporter
{
public:
    ModelImporter(ResourceManager* resourceManager);

    Entity Import(const std::string& path);

private:
    Entity ImportInternal(const std::string& path);

    static Mesh GenerateVertexData(aiMesh* assimpMesh);
    Model GenerateModel(aiMaterial* assimpMaterial, Mesh& mesh, const std::string& directory) const;
    void GenerateSceneGraph(Entity& entity, const aiScene* scene, const aiNode* assimpNode, const std::string& directory);

    ResourceManager* resourceManager;
};


#endif //OPENGLENGINE_MODELIMPORTER_H