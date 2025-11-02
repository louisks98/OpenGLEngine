//
// Created by louis on 10/23/25.
//

#include "Shader.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "HelperFunctions.h"
#include "glad/glad.h"

Shader::Shader(const std::string &pathVert, const std::string &pathFrag)
{
    const uint32_t vertId = glCreateShader(GL_VERTEX_SHADER);
    const std::string vertContent = ReadFile(pathVert);
    Compile(vertId, pathVert, vertContent);

    const uint32_t fragId = glCreateShader(GL_FRAGMENT_SHADER);
    const std::string fragContent = ReadFile(pathFrag);
    Compile(fragId, pathFrag, fragContent);

    vertex = vertId;
    fragment = fragId;
    program = glCreateProgram();
    Link();
}

void Shader::Compile(const uint32_t Id, const std::string &path, const std::string &content)
{
    const char* source = content.c_str();
    glShaderSource(Id, 1, &source, nullptr);
    glCompileShader(Id);

    int  success;
    char infoLog[512];
    glGetShaderiv(Id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(Id, 512, nullptr, infoLog);
        std::cout << "ERROR::COMPILATION_FAILED : path=" + path + "\n" << infoLog << std::endl;
    }
}


void Shader::Link() const
{
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    int  success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() const
{
    glUseProgram(program);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4 &value) const
{
    const int location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformMat3(const std::string &name, const glm::mat3 &value) const
{
    const int location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformVec3(const std::string &name, const glm::vec3 &value) const
{
    const int location = glGetUniformLocation(program, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniformFloat(const std::string &name, float value) const
{
    const int location = glGetUniformLocation(program, name.c_str());
    glUniform1f(location, value);
}




