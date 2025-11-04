//
// Created by louis on 10/25/25.
//

#include "glad/glad.h"
#include "Renderer.h"
#include "Model.h"

#include <utility>
#include <GLFW/glfw3.h>


Renderer::Renderer(std::vector<Model> meshes,Light directional, std::vector<Light> pLights, std::vector<Light> spLights) :
models(std::move(meshes)),
pointLights(std::move(pLights)),
spotLights(std::move(spLights)),
directionalLight(directional),
projection(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f))
{
    renderObjects = std::map<Model *, RenderObject>();
    view = glm::mat4(1.0f);
    camera = Camera();
    Initialize();
}

void Renderer::Initialize()
{
    glEnable(GL_DEPTH_TEST);
    renderObjects.clear();

    for (auto &model : models)
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

        // Vertex position attribute (location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        // Vertex normal attribute (location 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture coords attribute (location 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        auto renderObject = RenderObject();
        renderObject.VAO = VAO;
        renderObject.VBO = VBO;
        renderObject.EBO = EBO;

        renderObjects[&model] = renderObject;
    }
}

void Renderer::Update(const float time)
{
    view = camera.GetViewMatrix();
    for (auto [model, renderObject] : renderObjects)
    {
        Mesh& mesh = model->GetMesh();
        Transform& transform = model->GetTransform();
        transform.SetRotation(glm::vec3(0.0f, time * 50.0f, 0.0f));
    }
}

void Renderer::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto [model, renderObject] : renderObjects)
    {
        Mesh& mesh = model->GetMesh();
        Transform& transform = model->GetTransform();
        auto modelMatrix = transform.GetMatrix();
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

        model->Render(TransformMatrix{projection, view, modelMatrix, normalMatrix});
        Shader& shader = model->GetMaterial().GetShader();

        shader.SetUniformVec3("viewPos", camera.GetTransform().GetPosition());
        shader.SetLight(directionalLight);

        shader.SetUniformInt("numPointLights", pointLights.size());
        if (!pointLights.empty())
            shader.SetLights(pointLights);

        shader.SetUniformInt("numSpotLights", spotLights.size());
        if (!spotLights.empty())
            shader.SetLights(spotLights);


        glBindVertexArray(renderObject.VAO);
        glDrawElements(GL_TRIANGLES, mesh.GetIndices()->size(), GL_UNSIGNED_INT, nullptr);
    }
}
