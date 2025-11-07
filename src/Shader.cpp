//
// Created by louis on 10/23/25.
//

#include "Shader.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include "HelperFunctions.h"
#include "glad/glad.h"

#include <regex>
#include <utility>
inline  static std::string ProcessIncludes(const std::string& source, const std::string& directory)
{
    std::string result = source;
    std::regex includeRegex(R"###(#include\s+"([^"]+)")###");
    std::smatch match;

    while (std::regex_search(result, match, includeRegex))
    {
        const std::string includePath = directory + "/" + match[1].str();
        const std::string includeContent = ReadFile(includePath);

        // Recursively process includes in the included file
        const std::string processedInclude = ProcessIncludes(includeContent, directory);

        // Replace the #include directive with the file content
        result = result.substr(0, match.position()) + processedInclude + result.substr(match.position() + match.length());
    }

    return result;
}

Shader::Shader()
    : program(0), vertex(0), fragment(0)
{
}

Shader::Shader(const std::string &pathVert, const std::string &pathFrag)
    : program(0), vertex(0), fragment(0)
{
    const uint32_t vertId = glCreateShader(GL_VERTEX_SHADER);
    std::string vertContent = ReadFile(pathVert);

    // Get directory from path for resolving includes
    const std::string vertDir = std::filesystem::path(pathVert).parent_path().string();
    vertContent = ProcessIncludes(vertContent, vertDir);

    Compile(vertId, pathVert, vertContent);

    const uint32_t fragId = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragContent = ReadFile(pathFrag);

    const std::string fragDir = std::filesystem::path(pathFrag).parent_path().string();
    fragContent = ProcessIncludes(fragContent, fragDir);

    Compile(fragId, pathFrag, fragContent);

    vertex = vertId;
    fragment = fragId;
    program = glCreateProgram();
    Link();
}

Shader::~Shader()
{
    if (program != 0)
    {
        glDeleteProgram(program);
        program = 0;
    }
}

Shader::Shader(Shader&& other) noexcept
    : program(other.program),
      vertex(other.vertex),
      fragment(other.fragment),
      uniformLocationCache(std::move(other.uniformLocationCache))
{
    other.program = 0;
    other.vertex = 0;
    other.fragment = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        // Clean up existing resources
        if (program != 0)
        {
            glDeleteProgram(program);
        }

        // Transfer ownership
        program = other.program;
        vertex = other.vertex;
        fragment = other.fragment;
        uniformLocationCache = std::move(other.uniformLocationCache);

        // Reset other
        other.program = 0;
        other.vertex = 0;
        other.fragment = 0;
    }
    return *this;
}

int Shader::GetUniformLocation(const std::string& name) const
{
    // Check cache first
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end())
    {
        return it->second;
    }

    // Not in cache, query OpenGL
    const int location = glGetUniformLocation(program, name.c_str());
    uniformLocationCache[name] = location;
    return location;
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
    const int location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformMat3(const std::string &name, const glm::mat3 &value) const
{
    const int location = GetUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformVec3(const std::string &name, const glm::vec3 &value) const
{
    const int location = GetUniformLocation(name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniformFloat(const std::string &name, const float value) const
{
    const int location = GetUniformLocation(name);
    glUniform1f(location, value);
}

void Shader::SetUniformInt(const std::string &name, const int value) const
{
    const int location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::SetLight(const Light &light, const std::string &index) const
{
    switch (light.GetType())
    {
       case LightType::Directional:
            SetUniformVec3("directionalLight.direction", light.GetDirection());
            SetUniformVec3("directionalLight.color", light.GetColor());
            SetUniformFloat("directionalLight.intensity", light.GetIntensity());
            break;
        case LightType::Point:
            SetUniformVec3("pointLights[" + index + "].position", light.GetTransform().GetPosition());
            SetUniformVec3("pointLights[" + index + "].color", light.GetColor());
            SetUniformFloat("pointLights[" + index + "].intensity", light.GetIntensity());
            SetUniformFloat("pointLights[" + index + "].constant", Light::GetConstant());
            SetUniformFloat("pointLights[" + index + "].linear", light.GetLinear());
            SetUniformFloat("pointLights[" + index + "].quadratic", light.GetQuadratic());
            break;

        case LightType::Spot:
            SetUniformVec3("spotLights[" + index + "].position", light.GetTransform().GetPosition());
            SetUniformVec3("spotLights[" + index + "].spotDirection", light.GetDirection());
            SetUniformVec3("spotLights[" + index + "].color", light.GetColor());
            SetUniformFloat("spotLights[" + index + "].intensity", light.GetIntensity());
            SetUniformFloat("spotLights[" + index + "].cutoff", light.GetCutoff());
            break;
    };
}

void Shader::SetLights(const std::vector<Light>& lights) const
{
    for (size_t i = 0; i < lights.size(); i++)
        SetLight(lights[i], std::to_string(i));
}






