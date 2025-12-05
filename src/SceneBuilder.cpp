//
// Created by Louis on 12/4/2025.
//

#include "SceneBuilder.h"

SceneBuilder::SceneBuilder(Scene *scene, PrimitiveFactory *primitiveFactory, ModelImporter *modelImporter) : scene(scene), primitiveFactory(primitiveFactory), modelImporter(modelImporter) {}

void SceneBuilder::AddCube(const uint32_t matId, const Transform &transform) const
{
    const auto meshId = primitiveFactory->CreateCube();
    auto cubeModel = std::make_unique<Model>(Model(meshId, matId));
    cubeModel->SetTransform(transform);
    scene->AddEntity(std::move(cubeModel));
}

void SceneBuilder::AddSphere(const uint32_t matId, const Transform &transform) const
{
    const auto meshId = primitiveFactory->CreateSphere();
    auto sphereModel = std::make_unique<Model>(Model(meshId, matId));
    sphereModel->SetTransform(transform);
    scene->AddEntity(std::move(sphereModel));
}

void SceneBuilder::AddModel(const std::string &modelPath, const Transform &transform) const
{
    auto importedModel = modelImporter->Import(modelPath);
    importedModel.SetTransform(transform);
    scene->AddEntity(std::make_unique<Entity>(std::move(importedModel)));
}
