//
// Created by louis on 10/24/25.
//

#include "Texture.h"

#include <filesystem>
#include <iostream>
#include <utility>

#include "stb_image.h"
#include "glad/glad.h"

Texture::Texture(const TextureType type, const std::string &source)
{
    this->type = type;
    SourcePath = source;
    if (type == TextureType::Simple)
        LoadTexture(source);

    if (type == TextureType::Cubemap)
        LoadCubemapTexture(source);

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
    type = other.type;
    other.Id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        if (Id != 0)
        {
            glDeleteTextures(1, &Id);
        }

        Id = other.Id;
        type = other.type;
        SourcePath = std::move(other.SourcePath);
        other.Id = 0;
    }
    return *this;
}

void Texture::Bind(unsigned int textureUnit) const
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    if (type == TextureType::Cubemap)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
    }
    else if (type == TextureType::Simple)
    {
        glBindTexture(GL_TEXTURE_2D, Id);
    }
}

void Texture::LoadTexture(const std::string &source)
{
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

void Texture::LoadCubemapTexture(const std::string &directory)
{
    const auto directoryPath = std::filesystem::path(directory);

    if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath))
    {
        std::cout << "Cubemap directory not found: " << directory << std::endl;
        return;
    }
    const std::vector<std::string> faceNames = {
        "right", "left", "top", "bottom", "front", "back"
    };
    const std::vector<std::string> extensions = {".jpg", ".png"};

    glGenTextures(1, &Id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Id);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faceNames.size(); i++)
    {
        std::string filePath;
        bool found = false;

        for (const auto& ext : extensions)
        {
            auto testPath = directoryPath / (faceNames[i] + ext);
            if (std::filesystem::exists(testPath))
            {
                filePath = testPath.string();
                found = true;
                break;
            }
        }

        if (!found)
        {
            std::cout << "Cubemap face not found: " << faceNames[i] << " (tried .jpg, .png, .bmp, .tga)" << std::endl;
            continue;
        }

        unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load cubemap texture: " << filePath << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}



