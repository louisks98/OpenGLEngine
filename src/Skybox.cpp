//
// Created by Louis on 11/12/2025.
//

#include "Skybox.h"
#include "glad/glad.h"

Skybox::Skybox(const std::string &path)
{
    constexpr float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f
    };

    texture = Texture(TextureType::Cubemap, path);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    skyboxShader = Shader("shader/skybox_vs.glsl", "shader/skybox_fg.glsl", "skybox");
}

Skybox::Skybox(Skybox &&other) noexcept
    : VAO(other.VAO),
      VBO(other.VBO),
      skyboxShader(std::move(other.skyboxShader)),
      texture(std::move(other.texture))
{
    other.VAO = 0;
    other.VBO = 0;
}

Skybox::~Skybox()
{
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
}

Skybox &Skybox::operator=(Skybox &&other) noexcept {
    if (this != &other)
    {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);

        VAO = other.VAO;
        VBO = other.VBO;
        skyboxShader = std::move(other.skyboxShader);
        texture = std::move(other.texture);

        other.VAO = 0;
        other.VBO = 0;
    }

    return *this;
}

void Skybox::Render(const glm::mat4 &projection, const glm::mat4 &view) const
{
    auto skyboxView = glm::mat4(glm::mat3(view));
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    skyboxShader.Use();
    skyboxShader.SetUniformMat4("projection", projection);
    skyboxShader.SetUniformMat4("view", skyboxView);
    skyboxShader.SetUniformInt("skybox", 0);
    glBindVertexArray(VAO);
    texture.Bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
}

