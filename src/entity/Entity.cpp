//
// Created by Louis on 11/7/2025.
//

#include "Entity.h"

Entity::Entity(Entity&& other) noexcept
    : transform(std::move(other.transform)),
      children(std::move(other.children)),
      parent(other.parent),
      updateDelegate(std::move(other.updateDelegate))
{
    for (auto& child : children)
    {
        child->parent = this;
    }
    other.parent = nullptr;
}

Entity& Entity::operator=(Entity&& other) noexcept
{
    if (this != &other)
    {
        transform = std::move(other.transform);
        children = std::move(other.children);
        parent = other.parent;
        updateDelegate = std::move(other.updateDelegate);

        for (auto& child : children)
        {
            child->parent = this;
        }

        other.parent = nullptr;
    }
    return *this;
}

void Entity::Update(const float deltaTime)
{
    if (updateDelegate)
        updateDelegate(this, deltaTime);

    if (parent)
    {
        transform.UpdateMatrix(parent->GetTransform().GetMatrix());
    }
    else if (transform.IsDirty())
    {
        transform.UpdateMatrix();
    }

    for (const auto& child : children)
        child->Update(deltaTime);
}

void Entity::AddChild(std::unique_ptr<Entity> child)
{
    child->parent = this;
    children.push_back(std::move(child));
}
