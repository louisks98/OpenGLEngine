//
// Created by louis on 10/23/25.
//

#ifndef OPENGLENGINE_PRIMITIVE_H
#define OPENGLENGINE_PRIMITIVE_H
#include <cstdint>
#include <vector>

#include "glm/glm.hpp"

struct vertex
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};


class Mesh {
public:
    Mesh(const std::vector<vertex>& vertices, const std::vector<uint32_t>& indices);
    Mesh(const Mesh& mesh);

    [[nodiscard]]
    uint32_t GetId() const;

    [[nodiscard]]
    const std::vector<vertex>& GetVertices() const { return vertices; }

    [[nodiscard]]
    const std::vector<uint32_t>& GetIndices() const { return indices; }

private:
    uint32_t Id = rand();
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;
};

#endif //OPENGLENGINE_PRIMITIVE_H