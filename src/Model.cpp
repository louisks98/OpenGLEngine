//
// Created by louis on 10/26/25.
//

#include "Model.h"

Model::Model(const Mesh &mesh) : mesh(mesh)
{
    transform = Transform();
}

void Model::Render(const TransformMatrix &matrix) const
{
    const Shader& shader = material.GetShader();
    material.Render();
    shader.SetUniformMat4("projection", matrix.projection);
    shader.SetUniformMat4("view", matrix.view);
    shader.SetUniformMat4("model", matrix.model);
    shader.SetUniformMat3("normalMat", matrix.normal);
}
