//
// Created by louis on 10/23/25.
//

#ifndef OPENGLENGINE_PRIMITIVE_H
#define OPENGLENGINE_PRIMITIVE_H
#include <cstdint>
#include <vector>


class Primitive
{
public:
    Primitive(float vertices[], uint32_t vSize,  uint32_t indices[], uint32_t iSize);

    void Draw() const;
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};


#endif //OPENGLENGINE_PRIMITIVE_H