//
// Created by louis on 10/24/25.
//

#ifndef OPENGLENGINE_TEXTURE_H
#define OPENGLENGINE_TEXTURE_H
#include <string>


class Texture {
public:
    Texture() = default;
    explicit Texture(const std::string &source);
    ~Texture();

    // Delete copy operations (can't copy OpenGL textures safely)
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Move operations
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    [[nodiscard]]
    unsigned int GetId() const { return Id; };
    void Bind(unsigned int textureUnit = 0) const;
private:
    unsigned int Id = 0;
    std::string SourcePath;
};


#endif //OPENGLENGINE_TEXTURE_H