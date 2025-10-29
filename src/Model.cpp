//
// Created by louis on 10/26/25.
//

#include "Model.h"

Model::Model(const Mesh &mesh) : mesh(mesh) {
    transform = Transform();
}

void Model::SetTexture(Texture tex) { this->texture = std::move(tex);}
