//
// Created by louis on 10/25/25.
//

#ifndef OPENGLENGINE_RENDERER_H
#define OPENGLENGINE_RENDERER_H
#include <map>

#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"

struct RenderObject
{
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
};

class Renderer
{
public:
    Renderer(std::vector<Model> meshes, const ShaderProgram &shader);
    void Initialize();
    void Update(float time);
    void Draw() const;

private:
    std::map<Model*, RenderObject> renderObjects;
    std::vector<Model> models;
    ShaderProgram program;
    Camera camera;

    glm::mat4 projection;
    glm::mat4 view;
};


#endif //OPENGLENGINE_RENDERER_H