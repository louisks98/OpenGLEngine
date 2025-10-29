//
// Created by louis on 10/25/25.
//

#include "Renderer.h"
#include <utility>
#include <GLFW/glfw3.h>

#include "Model.h"
#include "glad/glad.h"

Renderer::Renderer(std::vector<Model> meshes, const ShaderProgram &shader)
{
    RenderObjects = std::map<Model *, RenderObject>();
    Models = std::move(meshes);
    Program = shader;
    Initialize();
}

void Renderer::Initialize()
{
    glEnable(GL_DEPTH_TEST);
    RenderObjects.clear();

    for (auto &model : Models)
    {
        Mesh& mesh = model.GetMesh();
        const auto vertices = mesh.GetVertices();
        const auto indices = mesh.GetIndices();

        uint32_t VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(),GL_STATIC_DRAW);

        // Element buffer object
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(uint32_t), indices->data(), GL_STATIC_DRAW);

        // Vertex position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        // Color position attribute
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
        //glEnableVertexAttribArray(1);

        // Texture coords attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        auto renderObject = RenderObject();
        renderObject.VAO = VAO;
        renderObject.VBO = VBO;
        renderObject.EBO = EBO;

        RenderObjects[&model] = renderObject;
    }
}

void Renderer::Draw() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


    for (auto [model, renderObject] : RenderObjects)
    {
        Mesh& mesh = model->GetMesh();
        Transform& transform = model->GetTransform();
        transform.SetRotation(glm::vec3(0.0f, static_cast<float>(glfwGetTime()) * 50.0f, 0.0f));
        auto matrix = transform.GetMatrix();
        auto texture = model->GetTexture();
        texture.Bind();

        Program.Use();
        Program.SetUniform("projection", projection);
        Program.SetUniform("view", view);
        Program.SetUniform("model", matrix);

        glBindVertexArray(renderObject.VAO);
        glDrawElements(GL_TRIANGLES, mesh.GetIndices()->size(), GL_UNSIGNED_INT, nullptr);
    }
}
