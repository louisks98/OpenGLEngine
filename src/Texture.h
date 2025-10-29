//
// Created by louis on 10/24/25.
//

#ifndef OPENGLENGINE_TEXTURE_H
#define OPENGLENGINE_TEXTURE_H
#include <string>


class Texture {
public:
    Texture() = default;
    Texture(const std::string &source);

    void Bind() const;
private:
    unsigned int Id;
    std::string SourcePath;
};


#endif //OPENGLENGINE_TEXTURE_H