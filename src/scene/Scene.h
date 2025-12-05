//
// Created by Louis on 11/7/2025.
//

#ifndef OPENGLENGINE_SCENE_H
#define OPENGLENGINE_SCENE_H
#include <memory>
#include <vector>

#include "../entity/Camera.h"
#include "../rendering/Skybox.h"

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

    void SetSkybox(std::unique_ptr<Skybox> skyboxPtr) {this->skybox = std::move(skyboxPtr);}
    Skybox* GetSkybox() const {return skybox.get();}

    void Update(float deltaTime) const;
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Model*> models;
    std::vector<Light*> lights;

    Camera camera = Camera();
    std::unique_ptr<Skybox> skybox;

    void RegisterEntityRecursive(Entity* entity);
    void TryAddModel(Entity* model);
    void TryAddLight(Entity* light);
};


#endif //OPENGLENGINE_SCENE_H