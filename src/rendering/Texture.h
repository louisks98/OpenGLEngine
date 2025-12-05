//
// Created by louis on 10/24/25.
//

#ifndef OPENGLENGINE_TEXTURE_H
#define OPENGLENGINE_TEXTURE_H
#include <string>

enum TextureType
{
    Simple,
    Cubemap,
};

class Texture {
public:
    Texture() = default;
    explicit Texture(TextureType type, const std::string &source);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    [[nodiscard]]
    unsigned int GetId() const { return Id; };
    void Bind(unsigned int textureUnit = 0) const;
private:
    unsigned int Id = 0;
    std::string SourcePath;
    TextureType type;

    void LoadTexture(const std::string &source);
    void LoadCubemapTexture(const std::string &directory);
};


#endif //OPENGLENGINE_TEXTURE_H