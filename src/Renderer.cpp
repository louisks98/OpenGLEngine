//
// Created by louis on 10/25/25.
//

#include "glad/glad.h"
#include "Renderer.h"
#include "Model.h"

#include <utility>
#include <GLFW/glfw3.h>

RenderObject::~RenderObject()
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
    if (EBO != 0)
    {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
}

RenderObject::RenderObject(RenderObject&& other) noexcept:
VAO(other.VAO),
VBO(other.VBO),
EBO(other.EBO),
indexCount(other.indexCount)
{
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.indexCount = 0;
}

RenderObject& RenderObject::operator=(RenderObject&& other) noexcept
{
    if (this != &other)
    {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (EBO != 0) glDeleteBuffers(1, &EBO);

        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        indexCount = other.indexCount;

        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.indexCount = 0;
    }
    return *this;
}

Renderer::Renderer(Scene* scene, ResourceManager* resourceManager) :
scene(scene),
resourceManager(resourceManager),
projection(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f))
{
    renderObjects = std::map<Model *, RenderObject>();
    view = glm::mat4(1.0f);
    Initialize();
}

void Renderer::Initialize()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    renderObjects.clear();

    const auto models = scene->GetModels();
    for ( auto model : models)
    {
        if (model == nullptr)
            continue;

        const uint32_t meshId = model->GetMesh();
        const Mesh* mesh = resourceManager->GetMesh(meshId);
        const auto& vertices = mesh->GetVertices();
        const auto& indices = mesh->GetIndices();

        uint32_t VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

        // Element buffer object
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

        // Vertex position attribute (location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, Position)));
        glEnableVertexAttribArray(0);

        // Vertex normal attribute (location 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, Normal)));
        glEnableVertexAttribArray(1);

        // Texture coords attribute (location 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, TexCoord)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        auto renderObject = RenderObject();
        renderObject.VAO = VAO;
        renderObject.VBO = VBO;
        renderObject.EBO = EBO;
        renderObject.indexCount = indices.size();

        renderObjects[model] = std::move(renderObject);
    }

    GatherLights();
}

void Renderer::GatherLights()
{
    directionalLight = nullptr;
    pointLights.clear();
    spotLights.clear();

    const auto lights = scene->GetLights();
    for (auto light : lights)
    {
        if (light == nullptr)
            continue;

        switch (light->GetType())
        {
            case LightType::Directional:
                directionalLight = light;
                break;
            case LightType::Point:
                pointLights.push_back(light);
                break;
            case LightType::Spot:
                spotLights.push_back(light);
                break;
        }
    }
}


void Renderer::Update(const float time)
{
    GatherLights();
    view = scene->GetCamera().GetViewMatrix();
    scene->Update(time);
}

void Renderer::Render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& [model, renderObject] : renderObjects)
    {
        Transform& transform = model->GetTransform();
        auto modelMatrix = transform.GetMatrix();
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

        const uint32_t materialId = model->GetMaterial();
        const Material* material = resourceManager->GetMaterial(materialId);

        const Shader* activeShader;
        if (USE_DEPTH_BUFFER_DEBUG)
        {
            activeShader = resourceManager->GetShader(resourceManager->depthBufferDebugShaderId);
            activeShader->Use();
        }
        else
        {
            activeShader = resourceManager->GetShader(material->GetShader());
            material->Render(activeShader);
        }

        activeShader->SetUniformMat4("projection", projection);
        activeShader->SetUniformMat4("view", view);
        activeShader->SetUniformMat4("model", modelMatrix);
        activeShader->SetUniformMat3("normalMat", normalMatrix);

        if (!USE_DEPTH_BUFFER_DEBUG)
        {
            activeShader->SetUniformVec3("viewPos", scene->GetCamera().GetTransform().GetPosition());
            activeShader->SetLight(directionalLight);

            activeShader->SetUniformInt("numPointLights", pointLights.size());
            if (!pointLights.empty())
                activeShader->SetLights(pointLights);

            activeShader->SetUniformInt("numSpotLights", spotLights.size());
            if (!spotLights.empty())
                activeShader->SetLights(spotLights);
        }


        glBindVertexArray(renderObject.VAO);
        glDrawElements(GL_TRIANGLES, renderObject.indexCount, GL_UNSIGNED_INT, nullptr);
    }
}
