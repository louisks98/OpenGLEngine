//
// Created by louis on 10/26/25.
//

#ifndef OPENGLENGINE_MODEL_H
#define OPENGLENGINE_MODEL_H
#include <utility>

#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"


class Model
{
public:
    explicit Model(const Mesh &mesh);

    [[nodiscard]]
    Mesh& GetMesh() { return mesh;}
    [[nodiscard]]
    const Transform& GetTransform() const { return transform;}
    [[nodiscard]]
    Texture& GetTexture() { return texture;}

    void SetTexture(Texture texture);

private:
    Transform transform;
    Mesh mesh;
    Texture texture;
};


#endif //OPENGLENGINE_MODEL_H