//
// Created by louis on 10/23/25.
//

#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram(Shader* vert, Shader* frag)
{
    vertex = vert;
    fragment = frag;
    program = glCreateProgram();
}

void ShaderProgram::Link() const
{
    glAttachShader(program, vertex->GetShaderId());
    glAttachShader(program, fragment->GetShaderId());
    glLinkProgram(program);

    int  success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    vertex->Delete();
    fragment->Delete();
}

void ShaderProgram::Use() const
{
    glUseProgram(program);
}


