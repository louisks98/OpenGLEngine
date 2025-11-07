//
// Created by louis on 10/25/25.
//

#ifndef OPENGLENGINE_RENDERER_H
#define OPENGLENGINE_RENDERER_H
#include <map>

#include "Camera.h"
#include "Light.h"
#include "Model.h"

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
    Renderer(std::vector<Entity*> entities, Light directional, std::vector<Light> pLights, std::vector<Light> spLights);
    void Initialize();
    void Update(float time);
    void Render();
    Camera& GetCamera() {return camera;};

private:
    Camera camera;

    std::map<Model*, RenderObject> renderObjects;
    std::vector<Entity *> Entities;
    Light directionalLight;
    std::vector<Light> pointLights;
    std::vector<Light> spotLights;

    glm::mat4 projection;
    glm::mat4 view{};
};


#endif //OPENGLENGINE_RENDERER_H