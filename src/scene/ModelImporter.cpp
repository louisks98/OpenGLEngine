//
// Created by Louis on 11/5/2025.
//

#include "ModelImporter.h"

#include <iostream>
#include <filesystem>
#include <chrono>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../rendering/Material.h"
#include "../rendering/Mesh.h"
#include "../entity/Model.h"

ModelImporter::ModelImporter(ResourceManager *resourceManager):resourceManager(resourceManager) {}


Entity ModelImporter::ImportInternal(const std::string& path)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenNormals |
        aiProcess_FlipUVs);

    auto importEndTime = std::chrono::high_resolution_clock::now();
    auto importDuration = std::chrono::duration_cast<std::chrono::milliseconds>(importEndTime - startTime);

    if (nullptr == scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::MODEL_IMPORT : path=" + path + "\n" << importer.GetErrorString() << std::endl;
        return {};
    }

    std::cout << "INFO::MODEL_IMPORT: Import time elapsed: " << importDuration.count() << "ms" << std::endl;

    std::string directory = std::filesystem::path(path).parent_path().string();

    if (scene->mNumMeshes == 0)
    {
        std::cout << "ERROR::MODEL_IMPORT : No meshes found in " + path << std::endl;
        return {};
    }

    std::cout << "INFO::MODEL_IMPORT: Found " << scene->mNumMeshes << " mesh(es)" << std::endl;


    Entity rootEntity;
    GenerateSceneGraph(rootEntity, scene, scene->mRootNode, directory);


    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "INFO::MODEL_IMPORT: Total time elapsed: " << duration.count() << "ms" << std::endl;
    return rootEntity;
}

Entity ModelImporter::Import(const std::string& path)
{
    return ImportInternal(path);
}

Mesh ModelImporter::GenerateVertexData(aiMesh* assimpMesh)
{
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;

    for (unsigned int j = 0; j < assimpMesh->mNumVertices; j++)
    {
        vertex vert{};

        aiVector3D meshVertex = assimpMesh->mVertices[j];
        vert.Position = glm::vec3(meshVertex.x, meshVertex.y, meshVertex.z);

        if (assimpMesh->HasNormals())
        {
            aiVector3D normal = assimpMesh->mNormals[j];
            vert.Normal = glm::vec3(normal.x, normal.y, normal.z);
        }
        else
        {
            vert.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        if (assimpMesh->HasTextureCoords(0))
        {
            aiVector3D texCoord = assimpMesh->mTextureCoords[0][j];
            vert.TexCoord = glm::vec2(texCoord.x, texCoord.y);
        }
        else
        {
            vert.TexCoord = glm::vec2(0.0f, 0.0f);
        }

        if (assimpMesh->HasVertexColors(0))
        {
            aiColor4D color = assimpMesh->mColors[0][j];
            vert.Color = glm::vec3(color.r, color.g, color.b);
        }
        else
        {
            vert.Color = glm::vec3(1.0f, 1.0f, 1.0f);
        }

        vertices.push_back(vert);
    }

    for (unsigned int k = 0; k < assimpMesh->mNumFaces; k++)
    {
        aiFace face = assimpMesh->mFaces[k];
        for (unsigned int l = 0; l < face.mNumIndices; l++)
        {
            indices.push_back(face.mIndices[l]);
        }
    }

    Mesh mesh(vertices, indices);
    return mesh;
}

Model ModelImporter::GenerateModel(aiMaterial *assimpMaterial, Mesh& mesh, const std::string& directory) const
{
    auto meshId = resourceManager->AddMesh(std::move(mesh));
    Model model(meshId);
    Material material;

    bool hasTextures = false;
    if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        aiString texturePath;
        if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
        {
            std::filesystem::path fullPath = std::filesystem::path(directory) / texturePath.C_Str();
            try
            {
                auto diffuseTexture = std::make_shared<Texture>(TextureType::Simple, fullPath.string());
                material.SetTextureProperty("material.diffuse", diffuseTexture);
                hasTextures = true;
            }
            catch (const std::exception& e)
            {
                std::cout << "WARNING::TEXTURE_LOAD_FAILED: " << fullPath.string() << " - " << e.what() << std::endl;
            }
        }
    }

    if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
    {
        aiString texturePath;
        if (assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == AI_SUCCESS)
        {
            std::filesystem::path fullPath = std::filesystem::path(directory) / texturePath.C_Str();
            try
            {
                auto specularTexture = std::make_shared<Texture>(TextureType::Simple, fullPath.string());
                material.SetTextureProperty("material.specular", specularTexture);
            }
            catch (const std::exception& e)
            {
                std::cout << "WARNING::TEXTURE_LOAD_FAILED: " << fullPath.string() << " - " << e.what() << std::endl;
            }
        }
    }

    if (!hasTextures)
    {
        aiColor4D diffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
        assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        material.SetColorProperty("material.mainColor", glm::vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a));

        aiColor4D specularColor(1.0f, 1.0f, 1.0f, 1.0f);
        assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        material.SetColorProperty("material.specular", glm::vec4(specularColor.r, specularColor.g, specularColor.b, specularColor.a));
    }

    float shininess = 32.0f;
    assimpMaterial->Get(AI_MATKEY_SHININESS, shininess);
    material.SetFloatProperty("material.shininess", shininess);

    if (hasTextures)
    {
        auto phongMapShaderId = resourceManager->GetShaderIndexByName("phong_maps");
        material.SetShader(phongMapShaderId);
        std::cout << "INFO::MODEL_IMPORT: Using textured shader (phong_maps)" << std::endl;
    }
    else if (!hasTextures)
    {
        auto phongShader = resourceManager->GetShaderIndexByName("phong");
        material.SetShader(phongShader);
        std::cout << "INFO::MODEL_IMPORT: Using solid color shader (phong)" << std::endl;
    }
    else
    {
        std::cout << "WARNING::MODEL_IMPORT: No appropriate shader found in pool" << std::endl;
    }

    auto materialId = resourceManager->AddMaterial(std::move(material));
    model.SetMaterial(materialId);

    return model;
}

void ModelImporter::GenerateSceneGraph(Entity& entity, const aiScene* scene, const aiNode *assimpNode, const std::string &directory)
{
    std::cout << "INFO::SCENE_GRAPH: Processing node \"" << assimpNode->mName.C_Str()
              << "\" with " << assimpNode->mNumMeshes << " mesh(es) and "
              << assimpNode->mNumChildren << " child(ren)" << std::endl;

    if (assimpNode->mNumMeshes > 0)
    {
        for (unsigned int j = 0; j < assimpNode->mNumMeshes; j++)
        {
            unsigned int meshIndex = assimpNode->mMeshes[j];
            aiMesh* sceneMesh = scene->mMeshes[meshIndex];

            std::cout << "  - Mesh " << meshIndex << ": \"" << sceneMesh->mName.C_Str() << "\"" << std::endl;

            Mesh mesh = GenerateVertexData(sceneMesh);

            // Check if material index is valid
            if (sceneMesh->mMaterialIndex >= 0 && sceneMesh->mMaterialIndex < scene->mNumMaterials)
            {
                Model model = GenerateModel(scene->mMaterials[sceneMesh->mMaterialIndex], mesh, directory);
                entity.AddChild(std::make_unique<Model>(std::move(model)));
            }
            else
            {
                std::cout << "WARNING::SCENE_GRAPH: Mesh \"" << sceneMesh->mName.C_Str()
                          << "\" has invalid material index " << sceneMesh->mMaterialIndex << std::endl;

                const auto meshId = resourceManager->AddMesh(std::move(mesh));
                entity.AddChild(std::make_unique<Model>(Model(meshId)));
            }
        }
    }

    for (unsigned int i = 0; i < assimpNode->mNumChildren; i++)
    {
        aiNode* childNode = assimpNode->mChildren[i];
        auto childEntity = std::make_unique<Entity>();

        GenerateSceneGraph(*childEntity, scene, childNode, directory);

        entity.AddChild(std::move(childEntity));
    }
}

