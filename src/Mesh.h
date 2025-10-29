//
// Created by louis on 10/23/25.
//

#ifndef OPENGLENGINE_PRIMITIVE_H
#define OPENGLENGINE_PRIMITIVE_H
#include <cstdint>
#include <vector>

#include "glm/glm.hpp"

class Mesh {
public:
    Mesh(float vertices[], uint32_t vSize,  uint32_t indices[], uint32_t iSize);
    Mesh(const Mesh& mesh);

    [[nodiscard]]
    uint32_t GetId() const;
    std::vector<float>* GetVertices();
    std::vector<uint32_t>* GetIndices();
private:
    uint32_t Id = rand();
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif //OPENGLENGINE_PRIMITIVE_H