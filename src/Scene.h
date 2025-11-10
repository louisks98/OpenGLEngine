//
// Created by Louis on 11/7/2025.
//

#ifndef OPENGLENGINE_SCENE_H
#define OPENGLENGINE_SCENE_H
#include <memory>
#include <vector>

#include "Camera.h"

class Entity;
class Light;
class Model;

class Scene
{
public:
    void AddEntity(std::unique_ptr<Entity> entity);
    const std::vector<std::unique_ptr<Entity>>& GetEntities() { return entities; }
    const std::vector<Model*>& GetModels(){ return models; }
    const std::vector<Light*>& GetLights() { return lights; }

    Camera& GetCamera() {return camera;};
    void Update(float deltaTime) const;
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Model*> models;
    std::vector<Light*> lights;

    Camera camera = Camera();

    void RegisterEntityRecursive(Entity* entity);
    void TryAddModel(Entity* model);
    void TryAddLight(Entity* light);
};


#endif //OPENGLENGINE_SCENE_H