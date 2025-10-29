//
// Created by louis on 10/23/25.
//

#include "Mesh.h"
#include "glad/glad.h"

Mesh::Mesh(float vertices[], uint32_t vSize, uint32_t indices[], uint32_t iSize)
{
    this->vertices = std::vector<float>{vertices, vertices + vSize};
    this->indices = std::vector<uint32_t>{indices, indices + iSize};
}

Mesh::Mesh(const Mesh &mesh) {
    vertices = mesh.vertices;
    indices = mesh.indices;
}

uint32_t Mesh::GetId() const
{
    return Id;
}

std::vector<float>* Mesh::GetVertices()
{
    return &vertices;
}

std::vector<uint32_t>* Mesh::GetIndices()
{
    return  &indices;
}
