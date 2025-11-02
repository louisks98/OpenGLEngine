//
// Created by louis on 10/23/25.
//

#ifndef OPENGLENGINE_SHADER_H
#define OPENGLENGINE_SHADER_H
#include <string>

#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>


class Shader
{
public:
    Shader() = default;
    Shader(const std::string &pathVert, const std::string &pathFrag);

    void Link() const;
    void Use() const;
    void SetUniformMat4(const std::string& name, const glm::mat4 &value) const;
    void SetUniformMat3(const std::string& name, const glm::mat3 &value) const;
    void SetUniformVec3(const std::string& name, const glm::vec3 &value) const;
    void SetUniformFloat(const std::string& name, float value) const;
private:
    unsigned int program;
    uint32_t vertex;
    uint32_t fragment;

    static void Compile(uint32_t Id, const std::string &path, const std::string &content);
};


#endif //OPENGLENGINE_SHADER_H