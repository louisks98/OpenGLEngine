//
// Created by Louis on 10/31/2025.
//

#ifndef OPENGLENGINE_MATERIAL_H
#define OPENGLENGINE_MATERIAL_H
#include <memory>
#include <utility>

#include "Shader.h"
#include "Texture.h"
#include "glm/vec3.hpp"


class Material
{
public:
    Material();

    [[nodiscard]]
    Shader& GetShader() const {return *shader;};
    void SetShader(const std::shared_ptr<Shader> &shdr) { shader = shdr;};

    [[nodiscard]]
    Texture& GetTexture() const { return *texture;}
    void SetTexture(const std::shared_ptr<Texture> &tex) {texture = tex;};

    void SetMainColor(const glm::vec3 newColor) {mainColor = newColor;};
    void SetSpecularColor(const glm::vec3 newSpecularColor) {specular = newSpecularColor;};
    void SetShininess(const float newShininess) {shininess = newShininess;};

    void Render() const;
private:
    glm::vec3 mainColor;
    glm::vec3 specular;
    float shininess;

    std::shared_ptr<Shader> shader = nullptr;
    std::shared_ptr<Texture> texture = nullptr;
};

#endif //OPENGLENGINE_MATERIAL_H