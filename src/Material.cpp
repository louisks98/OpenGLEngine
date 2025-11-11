//
// Created by Louis on 10/31/2025.
//

#include "Material.h"

Material::Material() : shaderId(-1), materialId(rand())
{
    SetIntProperty("material.type", MaterialType::Opaque);
}

void Material::SetColorProperty(const std::string &name, const glm::vec4 color)
{
    colorProperties[name] = color;
}

std::optional<glm::vec4> Material::GetColorProperty(const std::string &name) const
{
    if (colorProperties.contains(name))
        return colorProperties.at(name);

    return {};
}

void Material::SetFloatProperty(const std::string &name, const float value)
{
    floatProperties[name] = value;
}

std::optional<int32_t> Material::GetIntProperty(const std::string &name) const
{
    if (intProperties.contains(name))
        return intProperties.at(name);

    return {};
}

void Material::SetIntProperty(const std::string &name, const int32_t value)
{
    intProperties[name] = value;
}

std::optional<float> Material::GetFloatProperty(const std::string &name) const
{
    if (floatProperties.contains(name))
        return floatProperties.at(name);

    return {};
}

void Material::SetTextureProperty(const std::string &name, const std::shared_ptr<Texture> &texture)
{
    textureProperties[name] = texture;
}

std::optional<std::shared_ptr<Texture>> Material::GetTextureProperty(const std::string &name) const
{
    if (textureProperties.contains(name))
        return textureProperties.at(name);

    return {};
}

void Material::Render(const Shader* shader) const
{
    if (shader == nullptr)
        return;

    shader->Use();

    for (const auto &[name, value] : floatProperties)
        shader->SetUniformFloat(name, value);

    for (const auto &[name, value] : colorProperties)
        shader->SetUniformVec4(name, value);

    for (const auto &[name, value] : intProperties)
        shader->SetUniformInt(name, value);

    int textureUnit = 0;
    for (const auto &[name, value] : textureProperties)
    {
        value->Bind(textureUnit);
        shader->SetUniformInt(name, textureUnit);
        textureUnit++;
    }
}


