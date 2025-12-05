//
// Created by Louis on 11/12/2025.
//

#ifndef OPENGLENGINE_SKYBOX_H
#define OPENGLENGINE_SKYBOX_H
#include <memory>
#include <string>

#include "../scene/ResourceManager.h"


class Texture;

class Skybox
{
public:
    Skybox(const std::string &path);
    Skybox(Skybox&& other) noexcept;
    ~Skybox();
    Skybox& operator=(Skybox&& other) noexcept;

    void Render(const glm::mat4 &projection, const glm::mat4 &view) const;

private:
    uint32_t VAO, VBO;
    Shader skyboxShader;
    Texture texture;
};


#endif //OPENGLENGINE_SKYBOX_H