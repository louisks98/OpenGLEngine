//
// Created by Louis on 10/31/2025.
//

#include "Material.h"

Material::Material()
{
    mainColor = glm::vec3(1.0f);
    specular = glm::vec3(1.0f);
    shininess = 0.0f;
    texture = nullptr;
    shader = nullptr;
}

void Material::Render() const
{
    if (texture != nullptr)
        texture->Bind();

    if (shader == nullptr)
        return;

    shader->Use();
    shader->SetUniformVec3("material.mainColor", mainColor);
    shader->SetUniformVec3("material.specular", specular);
    shader->SetUniformFloat("material.shininess", shininess);
}


