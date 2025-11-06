//
// Created by Louis on 11/5/2025.
//

#include "ModelImporter.h"

#include <iostream>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"

static std::vector<Model> ImportInternal(const std::string& path, const ShaderPool* shaders = nullptr)
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

    std::vector<Model> models;
    models.reserve(scene->mNumMeshes);

    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
    {
        aiMesh* sceneMesh = scene->mMeshes[meshIndex];

        std::cout << "INFO::MODEL_IMPORT: Processing mesh " << (meshIndex + 1) << "/" << scene->mNumMeshes
                  << " (\"" << sceneMesh->mName.C_Str() << "\")" << std::endl;

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

    for (unsigned int j = 0; j < sceneMesh->mNumVertices; j++)
    {
        vertex vert{};

        aiVector3D meshVertex = sceneMesh->mVertices[j];
        vert.Position = glm::vec3(meshVertex.x, meshVertex.y, meshVertex.z);

        if (sceneMesh->HasNormals())
        {
            aiVector3D normal = sceneMesh->mNormals[j];
            vert.Normal = glm::vec3(normal.x, normal.y, normal.z);
        }
        else
        {
            vert.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        if (sceneMesh->HasTextureCoords(0))
        {
            aiVector3D texCoord = sceneMesh->mTextureCoords[0][j];
            vert.TexCoord = glm::vec2(texCoord.x, texCoord.y);
        }
        else
        {
            vert.TexCoord = glm::vec2(0.0f, 0.0f);
        }

        if (sceneMesh->HasVertexColors(0))
        {
            aiColor4D color = sceneMesh->mColors[0][j];
            vert.Color = glm::vec3(color.r, color.g, color.b);
        }
        else
        {
            vert.Color = glm::vec3(1.0f, 1.0f, 1.0f);
        }

        vertices.push_back(vert);
    }

        for (unsigned int k = 0; k < sceneMesh->mNumFaces; k++)
        {
            aiFace face = sceneMesh->mFaces[k];
            for (unsigned int l = 0; l < face.mNumIndices; l++)
            {
                indices.push_back(face.mIndices[l]);
            }
        }

        Mesh mesh(vertices, indices);
        Model model(mesh);

        if (sceneMesh->mMaterialIndex >= 0 && sceneMesh->mMaterialIndex < scene->mNumMaterials)
        {
            aiMaterial* assimpMaterial = scene->mMaterials[sceneMesh->mMaterialIndex];
            Material material;

            bool hasTextures = false;
            if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString texturePath;
                if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
                {
                    std::string fullPath = directory + "/" + std::string(texturePath.C_Str());
                    try
                    {
                        auto diffuseTexture = std::make_shared<Texture>(fullPath);
                        material.SetTextureProperty("material.diffuse", diffuseTexture);
                        hasTextures = true;
                    }
                    catch (const std::exception& e)
                    {
                        std::cout << "WARNING::TEXTURE_LOAD_FAILED: " << fullPath << " - " << e.what() << std::endl;
                    }
                }
            }

            if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
            {
                aiString texturePath;
                if (assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == AI_SUCCESS)
                {
                    std::string fullPath = directory + "/" + std::string(texturePath.C_Str());
                    try
                    {
                        auto specularTexture = std::make_shared<Texture>(fullPath);
                        material.SetTextureProperty("material.specular", specularTexture);
                    }
                    catch (const std::exception& e)
                    {
                        std::cout << "WARNING::TEXTURE_LOAD_FAILED: " << fullPath << " - " << e.what() << std::endl;
                    }
                }
            }

            if (!hasTextures)
            {
                aiColor3D diffuseColor(1.0f, 1.0f, 1.0f);
                assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
                material.SetColorProperty("material.mainColor", glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b));

                aiColor3D specularColor(1.0f, 1.0f, 1.0f);
                assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
                material.SetColorProperty("material.specular", glm::vec3(specularColor.r, specularColor.g, specularColor.b));
            }

            float shininess = 32.0f;
            assimpMaterial->Get(AI_MATKEY_SHININESS, shininess);
            material.SetFloatProperty("material.shininess", shininess);

            if (shaders != nullptr)
            {
                if (hasTextures && shaders->phongMapsShader != nullptr)
                {
                    material.SetShader(shaders->phongMapsShader);
                    std::cout << "INFO::MODEL_IMPORT: Using textured shader (phong_maps)" << std::endl;
                }
                else if (!hasTextures && shaders->phongShader != nullptr)
                {
                    material.SetShader(shaders->phongShader);
                    std::cout << "INFO::MODEL_IMPORT: Using solid color shader (phong)" << std::endl;
                }
                else
                {
                    std::cout << "WARNING::MODEL_IMPORT: No appropriate shader found in pool" << std::endl;
                }
            }

            model.SetMaterial(material);
        }

        models.push_back(model);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "INFO::MODEL_IMPORT: Total time elapsed: " << duration.count() << "ms" << std::endl;
    std::cout << "INFO::MODEL_IMPORT: Successfully loaded " << models.size() << " model(s)" << std::endl;

    return models;
}

std::vector<Model> ModelImporter::Import(const std::string& path, const ShaderPool& shaders)
{
    return ImportInternal(path, &shaders);
}

std::vector<Model> ModelImporter::Import(const std::string& path)
{
    return ImportInternal(path, nullptr);
}
