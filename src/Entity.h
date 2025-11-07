//
// Created by Louis on 10/30/2025.
//

#ifndef OPENGLENGINE_TRANSFORMABLE_H
#define OPENGLENGINE_TRANSFORMABLE_H
#include "Transform.h"
#include <memory>
#include <list>


class Entity {
    std::list<std::unique_ptr<Entity>> children;
    Entity* parent = nullptr;

    public:
    Entity() = default;
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;
    virtual ~Entity() = default;

    [[nodiscard]]
    Transform& GetTransform() { return transform;}

    [[nodiscard]]
    const Transform& GetTransform() const { return transform;}

    void AddChild(std::unique_ptr<Entity> child)
    {
        child->parent = this;
        children.push_back(std::move(child));
    }

    void UpdateTransformAndChildren()
    {
        if (transform.IsDirty())
        {
            if (parent)
                transform.UpdateMatrix(parent->GetTransform().GetMatrix());
            else
                transform.UpdateMatrix();

            for (const auto& child : children)
                child->UpdateTransformAndChildren();

            return;
        }

        for (const auto& child : children)
            child->UpdateTransformAndChildren();
    }

protected:
    Transform transform;
};


#endif //OPENGLENGINE_TRANSFORMABLE_H