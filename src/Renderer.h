//
// Created by louis on 10/25/25.
//

#ifndef OPENGLENGINE_RENDERER_H
#define OPENGLENGINE_RENDERER_H
#include <map>

#include "Light.h"
#include "Model.h"
#include "ResourceManager.h"
#include "Scene.h"

struct RenderObject
{
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    uint32_t EBO = 0;
    size_t indexCount = 0;

    RenderObject() = default;
    ~RenderObject();

    RenderObject(const RenderObject&) = delete;
    RenderObject& operator=(const RenderObject&) = delete;

    RenderObject(RenderObject&& other) noexcept;
    RenderObject& operator=(RenderObject&& other) noexcept;
};

class Renderer
{
public:
    Renderer(Scene* scene, ResourceManager* resourceManager);
    void Initialize();
    void Update(float time);
    void Render();

    bool USE_DEPTH_BUFFER_DEBUG = false;
private:
    Scene* scene;
    ResourceManager* resourceManager;
    std::map<Model*, RenderObject> renderObjects;
    std::vector<Model*> opaqueModels;
    std::vector<Model*> transparentModels;
    Light* directionalLight;
    std::vector<Light*> pointLights;
    std::vector<Light*> spotLights;

    glm::mat4 projection;
    glm::mat4 view{};

    void GatherLights();
    void GatherOpaqueModels();
    void GatherTransparentModels();
    void GatherModels(MaterialType type, std::vector<Model*>* models);

    void RenderModel(Model* model, const RenderObject& renderObject) const;
};


#endif //OPENGLENGINE_RENDERER_H