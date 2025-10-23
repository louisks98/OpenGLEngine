//
// Created by louis on 10/23/25.
//

#include "Shader.h"
#include <iostream>
#include <utility>
#include "HelperFunctions.h"
#include "glad/glad.h"

std::string ShaderTypeToString(ShaderType type)
{
    switch (type)
    {
        case VERTEX: return "VERTEX";
        case FRAGMENT: return "FRAGMENT";
    }
    return {};
}


Shader::Shader(std::string name, ShaderType type, const std::string& path)
{
    Id = glCreateShader(type);
    Name = std::move(name);
    Type = type;
    Path = path;
    const std::string fileContent = ReadFile(Path);
    Content = fileContent;
}

void Shader::Delete() const
{
    glDeleteShader(Id);
}

unsigned int Shader::GetShaderId() const
{
    return Id;
}

void Shader::Compile() const
{
    const char* source = Content.c_str();
    glShaderSource(Id, 1, &source, nullptr);
    glCompileShader(Id);

    int  success;
    char infoLog[512];
    glGetShaderiv(Id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(Id, 512, nullptr, infoLog);
        std::cout << "ERROR::COMPILATION_FAILED : Name=" + Name + "Type=" + ShaderTypeToString(Type) + "Path=" + Path + "\n" << infoLog << std::endl;
    }
}




