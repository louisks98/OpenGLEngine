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
    Renderer(std::vector<Model> meshes, std::vector<Light> lightsParam);
    void Initialize();
    void Update(float time);
    void Render();
    Camera& GetCamera() {return camera;};

private:
    Camera camera;

    std::map<Model*, RenderObject> renderObjects;
    std::vector<Model> models;
    std::vector<Light> lights;

    glm::mat4 projection;
    glm::mat4 view{};
};


#endif //OPENGLENGINE_RENDERER_H