//
// Created by louis on 10/25/25.
//

#ifndef OPENGLENGINE_RENDERER_H
#define OPENGLENGINE_RENDERER_H
#include <map>
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
    void Draw() const;

private:
    std::map<Model*, RenderObject> RenderObjects;
    std::vector<Model> Models;
    ShaderProgram Program;
};


#endif //OPENGLENGINE_RENDERER_H