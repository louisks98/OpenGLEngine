//
// Created by louis on 10/23/25.
//

#ifndef OPENGLENGINE_SHADER_H
#define OPENGLENGINE_SHADER_H
#include <string>
#include <unordered_map>

#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Light.h"


class Shader
{
public:
    Shader();
    Shader(const std::string &pathVert, const std::string &pathFrag, const std::string &name);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Move operations
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void Link() const;
    void Use() const;
    void SetUniformMat4(const std::string& name, const glm::mat4 &value) const;
    void SetUniformMat3(const std::string& name, const glm::mat3 &value) const;
    void SetUniformVec3(const std::string& name, const glm::vec3 &value) const;
    void SetUniformFloat(const std::string& name, float value) const;
    void SetUniformInt(const std::string& name, int value) const;
    void SetLight(const Light* light, const std::string &index = "") const;
    void SetLights(const std::vector<Light*>& lights) const;

    std::string GetShaderName() const {return name;};


private:
    unsigned int program = 0;
    uint32_t vertex = 0;
    uint32_t fragment = 0;
    std::string name = "";

    mutable std::unordered_map<std::string, int> uniformLocationCache;

    static void Compile(uint32_t Id, const std::string &path, const std::string &content);
    int GetUniformLocation(const std::string& name) const;
};


#endif //OPENGLENGINE_SHADER_H