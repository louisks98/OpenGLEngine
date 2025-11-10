//
// Created by louis on 10/26/25.
//

#include "Model.h"

Model::Model(const uint32_t mesh) : meshId(mesh), materialId(0){}

Model::Model(const uint32_t mesh, const uint32_t material) : meshId(mesh), materialId(material){}
