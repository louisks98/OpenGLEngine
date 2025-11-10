//
// Created by Louis on 11/7/2025.
//

#include "Scene.h"

#include "Light.h"
#include "Model.h"
#include <memory>

void Scene::AddEntity(std::unique_ptr<Entity> entity)
{
    RegisterEntityRecursive(entity.get());
    entities.push_back(std::move(entity));
}

void Scene::RegisterEntityRecursive(Entity* entity)
{
    if (entity == nullptr)
        return;

    TryAddLight(entity);
    TryAddModel(entity);

    const auto& children = entity->GetChildren();
    for (const auto& child : children)
    {
        RegisterEntityRecursive(child.get());
    }
}

void Scene::TryAddModel(Entity* entity)
{
    if (dynamic_cast<Model*>(entity) != nullptr)
    {
        const auto model = dynamic_cast<Model*>(entity);
        models.push_back(model);
    }
}

void Scene::TryAddLight(Entity* entity)
{
    if (dynamic_cast<Light*>(entity) != nullptr)
    {
        const auto light = dynamic_cast<Light*>(entity);
        lights.push_back(light);
    }
}

void Scene::Update(const float deltaTime) const
{
    for (auto& entity : entities)
        entity->Update(deltaTime);
}


