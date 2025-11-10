//
// Created by Louis on 10/30/2025.
//

#ifndef OPENGLENGINE_TRANSFORMABLE_H
#define OPENGLENGINE_TRANSFORMABLE_H

#include "Transform.h"
#include <memory>

class Entity {


    public:
    Entity() = default;
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&& other) noexcept;
    Entity& operator=(Entity&& other) noexcept;
    virtual ~Entity() = default;

    [[nodiscard]]
    Transform& GetTransform() { return transform;}
    [[nodiscard]]
    const Transform& GetTransform() const { return transform;}

    const std::vector<std::unique_ptr<Entity>>& GetChildren() const { return children;}
    void AddChild(std::unique_ptr<Entity> child);
    void Update(float deltaTime);
    void SetUpdateDelegate(const std::function<void(Entity*, float)> &delegate) {updateDelegate = delegate;};
    void ClearUpdateDelegate() { updateDelegate = nullptr; };

protected:
    Transform transform = Transform();

private:
    std::vector<std::unique_ptr<Entity>> children = std::vector<std::unique_ptr<Entity>>();
    Entity* parent = nullptr;

    std::function<void(Entity*, float)> updateDelegate;
};


#endif //OPENGLENGINE_TRANSFORMABLE_H