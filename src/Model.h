//
// Created by louis on 10/26/25.
//

#ifndef OPENGLENGINE_MODEL_H
#define OPENGLENGINE_MODEL_H

#include "Material.h"
#include "Mesh.h"
#include "Entity.h"

struct TransformMatrix
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat3 normal;
};

class Model : public Entity
{
public:
    explicit Model(const uint32_t mesh);
    explicit Model(const uint32_t mesh, const uint32_t material);

    [[nodiscard]]
    uint32_t GetMesh() const { return meshId;}
    void SetMaterial(const uint32_t mat) {materialId = mat;};
    uint32_t GetMaterial() const { return materialId;}

private:
    uint32_t meshId;
    uint32_t materialId;
};


#endif //OPENGLENGINE_MODEL_H