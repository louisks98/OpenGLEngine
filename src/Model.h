//
// Created by louis on 10/26/25.
//

#ifndef OPENGLENGINE_MODEL_H
#define OPENGLENGINE_MODEL_H

#include "Material.h"
#include "Mesh.h"
#include "Transformable.h"

struct TransformMatrix
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat3 normal;
};

class Model : public Transformable
{
public:
    explicit Model(const Mesh &mesh);

    [[nodiscard]]
    Mesh& GetMesh() { return mesh;}
    void SetMaterial(const Material& mat) {material = mat;};
    Material& GetMaterial() { return material;}
    void Render(const TransformMatrix &matrix) const;

private:
    Mesh mesh;
    Material material;
};


#endif //OPENGLENGINE_MODEL_H