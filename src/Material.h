//
// Created by Louis on 10/31/2025.
//

#ifndef OPENGLENGINE_MATERIAL_H
#define OPENGLENGINE_MATERIAL_H
#include <map>
#include <memory>
#include <optional>

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
    std::optional<glm::vec3> GetColorProperty(const std::string &name) const;
    void SetColorProperty(const std::string &name, glm::vec3 color);

    [[nodiscard]]
    std::optional<float> GetFloatProperty(const std::string &name) const;
    void SetFloatProperty(const std::string &name, float value);

    [[nodiscard]]
    std::optional<std::shared_ptr<Texture>> GetTextureProperty(const std::string &name) const;
    void SetTextureProperty(const std::string &name, const std::shared_ptr<Texture> &texture);

    void Render() const;
private:
    std::map<std::string, glm::vec3> colorProperties;
    std::map<std::string, float> floatProperties;
    std::map<std::string, std::shared_ptr<Texture>> textureProperties;

    std::shared_ptr<Shader> shader = nullptr;
};

#endif //OPENGLENGINE_MATERIAL_H