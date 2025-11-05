//
// Created by louis on 10/26/25.
//

#include "PrimitiveFactory.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Model PrimitiveFactory::CreateCube()
{
    std::vector<vertex> vertices = {
        // Front face (Normal: 0, 0, 1)
        {{-0.5, -0.5,  0.5}, {1.0, 1.0, 1.0}, { 0.0,  0.0,  1.0}, {0.0, 0.0}}, // 0
        {{ 0.5, -0.5,  0.5}, {1.0, 1.0, 1.0}, { 0.0,  0.0,  1.0}, {1.0, 0.0}}, // 1
        {{ 0.5,  0.5,  0.5}, {1.0, 1.0, 1.0}, { 0.0,  0.0,  1.0}, {1.0, 1.0}}, // 2
        {{-0.5,  0.5,  0.5}, {1.0, 1.0, 1.0}, { 0.0,  0.0,  1.0}, {0.0, 1.0}}, // 3

        // Back face (Normal: 0, 0, -1)
        {{-0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}, { 0.0,  0.0, -1.0}, {1.0, 0.0}}, // 4
        {{-0.5,  0.5, -0.5}, {1.0, 1.0, 1.0}, { 0.0,  0.0, -1.0}, {1.0, 1.0}}, // 5
        {{ 0.5,  0.5, -0.5}, {1.0, 1.0, 1.0}, { 0.0,  0.0, -1.0}, {0.0, 1.0}}, // 6
        {{ 0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}, { 0.0,  0.0, -1.0}, {0.0, 0.0}}, // 7

        // Top face (Normal: 0, 1, 0)
        {{-0.5,  0.5, -0.5}, {1.0, 1.0, 1.0}, { 0.0,  1.0,  0.0}, {0.0, 1.0}}, // 8
        {{-0.5,  0.5,  0.5}, {1.0, 1.0, 1.0}, { 0.0,  1.0,  0.0}, {0.0, 0.0}}, // 9
        {{ 0.5,  0.5,  0.5}, {1.0, 1.0, 1.0}, { 0.0,  1.0,  0.0}, {1.0, 0.0}}, // 10
        {{ 0.5,  0.5, -0.5}, {1.0, 1.0, 1.0}, { 0.0,  1.0,  0.0}, {1.0, 1.0}}, // 11

        // Bottom face (Normal: 0, -1, 0)
        {{-0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}, { 0.0, -1.0,  0.0}, {1.0, 1.0}}, // 12
        {{ 0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}, { 0.0, -1.0,  0.0}, {0.0, 1.0}}, // 13
        {{ 0.5, -0.5,  0.5}, {1.0, 1.0, 1.0}, { 0.0, -1.0,  0.0}, {0.0, 0.0}}, // 14
        {{-0.5, -0.5,  0.5}, {1.0, 1.0, 1.0}, { 0.0, -1.0,  0.0}, {1.0, 0.0}}, // 15

        // Right face (Normal: 1, 0, 0)
        {{ 0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}, { 1.0,  0.0,  0.0}, {1.0, 0.0}}, // 16
        {{ 0.5,  0.5, -0.5}, {1.0, 1.0, 1.0}, { 1.0,  0.0,  0.0}, {1.0, 1.0}}, // 17
        {{ 0.5,  0.5,  0.5}, {1.0, 1.0, 1.0}, { 1.0,  0.0,  0.0}, {0.0, 1.0}}, // 18
        {{ 0.5, -0.5,  0.5}, {1.0, 1.0, 1.0}, { 1.0,  0.0,  0.0}, {0.0, 0.0}}, // 19

        // Left face (Normal: -1, 0, 0)
        {{-0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}, {-1.0,  0.0,  0.0}, {0.0, 0.0}}, // 20
        {{-0.5, -0.5,  0.5}, {1.0, 1.0, 1.0}, {-1.0,  0.0,  0.0}, {1.0, 0.0}}, // 21
        {{-0.5,  0.5,  0.5}, {1.0, 1.0, 1.0}, {-1.0,  0.0,  0.0}, {1.0, 1.0}}, // 22
        {{-0.5,  0.5, -0.5}, {1.0, 1.0, 1.0}, {-1.0,  0.0,  0.0}, {0.0, 1.0}}  // 23
    };

    std::vector<uint32_t> indices = {
        0,  1,  2,      0,  2,  3,    // front
        4,  5,  6,      4,  6,  7,    // back
        8,  9,  10,     8,  10, 11,   // top
        12, 13, 14,     12, 14, 15,   // bottom
        16, 17, 18,     16, 18, 19,   // right
        20, 21, 22,     20, 22, 23    // left
    };

    const auto mesh = Mesh(vertices, indices);
    auto model = Model(mesh);

    return model;
}

Model PrimitiveFactory::CreateSphere()
{
    const float radius = 0.5f;
    const unsigned int segments = 36;  // Longitude divisions
    const unsigned int stacks = 18;    // Latitude divisions

    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;

    // Generate vertices
    for (unsigned int stack = 0; stack <= stacks; ++stack)
    {
        const float phi = M_PI * static_cast<float>(stack) / static_cast<float>(stacks);  // 0 to π
        const float y = radius * cos(phi);
        const float stackRadius = radius * sin(phi);

        for (unsigned int segment = 0; segment <= segments; ++segment)
        {
            const float theta = 2.0f * M_PI * static_cast<float>(segment) / static_cast<float>(segments);  // 0 to 2π

            // Position
            const float x = stackRadius * cos(theta);
            const float z = stackRadius * sin(theta);

            // Normal (normalized position vector for a sphere centered at origin)
            const float nx = x / radius;
            const float ny = y / radius;
            const float nz = z / radius;

            // Texture coordinates
            const float u = static_cast<float>(segment) / static_cast<float>(segments);
            const float v = static_cast<float>(stack) / static_cast<float>(stacks);

            // Create vertex
            vertex vert;
            vert.Position = glm::vec3(x, y, z);
            vert.Color = glm::vec3(1.0f, 1.0f, 1.0f);
            vert.Normal = glm::vec3(nx, ny, nz);
            vert.TexCoord = glm::vec2(u, v);
            vertices.push_back(vert);
        }
    }

    // Generate indices for triangle strips
    for (unsigned int stack = 0; stack < stacks; ++stack)
    {
        for (unsigned int segment = 0; segment < segments; ++segment)
        {
            const unsigned int first = stack * (segments + 1) + segment;
            const unsigned int second = first + segments + 1;

            // First triangle
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            // Second triangle
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    const auto mesh = Mesh(vertices, indices);
    auto model = Model(mesh);

    return model;
}
