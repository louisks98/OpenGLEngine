//
// Created by louis on 10/23/25.
//

#ifndef OPENGLENGINE_SHADERPROGRAM_H
#define OPENGLENGINE_SHADERPROGRAM_H
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>


class ShaderProgram
{
public:
    ShaderProgram() = default;
    ShaderProgram(Shader* vert, Shader* frag);

    void Link() const;
    void Use() const;
    void SetUniform(const std::string& name, const glm::mat4 &value) const;
private:
    unsigned int program;
    Shader* vertex;
    Shader* fragment;
};


#endif //OPENGLENGINE_SHADERPROGRAM_H