//
// Created by louis on 10/24/25.
//

#include "Texture.h"

#include <iostream>
#include <utility>

#include "stb_image.h"
#include "glad/glad.h"

Texture::Texture(const std::string &source)
{
    SourcePath = source;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(source.c_str(), &width, &height, &nrChannels, 0);

    glGenTextures(1, &Id);
    glBindTexture(GL_TEXTURE_2D, Id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture: " << source << std::endl;
    }
}

Texture::~Texture()
{
    if (Id != 0)
    {
        glDeleteTextures(1, &Id);
        Id = 0;
    }
}

Texture::Texture(Texture&& other) noexcept
    : Id(other.Id),
      SourcePath(std::move(other.SourcePath))
{
    other.Id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        // Clean up existing resources
        if (Id != 0)
        {
            glDeleteTextures(1, &Id);
        }

        // Transfer ownership
        Id = other.Id;
        SourcePath = std::move(other.SourcePath);

        // Reset other
        other.Id = 0;
    }
    return *this;
}

void Texture::Bind(unsigned int textureUnit) const
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, Id);
}

