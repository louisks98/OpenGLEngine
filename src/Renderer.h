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
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
};

class Renderer
{
public:
    Renderer(std::vector<Model> meshes, Light directional, std::vector<Light> pLights, std::vector<Light> spLights);
    void Initialize();
    void Update(float time);
    void Render();
    Camera& GetCamera() {return camera;};

private:
    Camera camera;

    std::map<Model*, RenderObject> renderObjects;
    std::vector<Model> models;
    Light directionalLight;
    std::vector<Light> pointLights;
    std::vector<Light> spotLights;

    glm::mat4 projection;
    glm::mat4 view{};
};


#endif //OPENGLENGINE_RENDERER_H