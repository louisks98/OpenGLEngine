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

enum MaterialType
{
    Opaque,
    Translucent,
};


class Material
{
public:
    Material();

    [[nodiscard]]
    uint32_t GetId() const {return materialId;};

    [[nodiscard]]
    uint32_t GetShader() const {return shaderId;};
    void SetShader(const uint32_t shdr) { shaderId = shdr;};

    [[nodiscard]]
    std::optional<glm::vec4> GetColorProperty(const std::string &name) const;
    void SetColorProperty(const std::string &name, glm::vec4 color);

    [[nodiscard]]
    std::optional<float> GetFloatProperty(const std::string &name) const;
    void SetFloatProperty(const std::string &name, float value);

    [[nodiscard]]
    std::optional<int32_t> GetIntProperty(const std::string &name) const;
    void SetIntProperty(const std::string &name, int32_t value);

    [[nodiscard]]
    std::optional<std::shared_ptr<Texture>> GetTextureProperty(const std::string &name) const;
    void SetTextureProperty(const std::string &name, const std::shared_ptr<Texture> &texture);

    void Render(const Shader* shader) const;
private:
    std::map<std::string, glm::vec4> colorProperties;
    std::map<std::string, float> floatProperties;
    std::map<std::string, int> intProperties;
    std::map<std::string, std::shared_ptr<Texture>> textureProperties;

    uint32_t shaderId;
    uint32_t materialId;
};

#endif //OPENGLENGINE_MATERIAL_H