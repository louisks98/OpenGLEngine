//
// Created by louis on 10/23/25.
//

#ifndef OPENGLENGINE_HELPERFUNCTIONS_H
#define OPENGLENGINE_HELPERFUNCTIONS_H
#include <fstream>
#include <iterator>
#include <string>

inline std::string ReadFile(const std::string& path)
{
    std::ifstream file{path};
    std::string contents{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    return contents;
}

#endif //OPENGLENGINE_HELPERFUNCTIONS_H