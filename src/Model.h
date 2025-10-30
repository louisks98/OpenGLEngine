//
// Created by louis on 10/26/25.
//

#ifndef OPENGLENGINE_MODEL_H
#define OPENGLENGINE_MODEL_H

#include "Mesh.h"
#include "Texture.h"
#include "Transformable.h"


class Model : public Transformable
{
public:
    explicit Model(const Mesh &mesh);

    [[nodiscard]]
    Mesh& GetMesh() { return mesh;}
    [[nodiscard]]
    Texture& GetTexture() { return texture;}

    void SetTexture(Texture texture);

private:
    Mesh mesh;
    Texture texture;
};


#endif //OPENGLENGINE_MODEL_H