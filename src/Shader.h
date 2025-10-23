//
// Created by louis on 10/23/25.
//

#ifndef OPENGLENGINE_SHADER_H
#define OPENGLENGINE_SHADER_H
#include <string>

#include "glad/glad.h"

enum ShaderType
{
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
};

class Shader
{
public:
    Shader(std::string name, ShaderType type, const std::string& path);

    [[nodiscard]]
    unsigned int GetShaderId() const;
    void Delete() const;

    void Compile() const;

private:
    unsigned int Id;
    std::string Name;
    ShaderType Type;
    std::string Path;
    std::string Content;
};


#endif //OPENGLENGINE_SHADER_H