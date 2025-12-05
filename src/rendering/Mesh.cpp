//
// Created by louis on 10/23/25.
//

#include "Mesh.h"
#include "glad/glad.h"

Mesh::Mesh(const std::vector<vertex>& vertices, const std::vector<uint32_t>& indices)
    : vertices(vertices), indices(indices)
{
}

Mesh::Mesh(const Mesh &mesh)
    : vertices(mesh.vertices), indices(mesh.indices)
{
}

uint32_t Mesh::GetId() const
{
    return Id;
}
