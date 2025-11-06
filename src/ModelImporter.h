//
// Created by Louis on 11/5/2025.
//

#ifndef OPENGLENGINE_MODELIMPORTER_H
#define OPENGLENGINE_MODELIMPORTER_H
#include <optional>
#include <string>
#include <memory>
#include <utility>
#include <vector>


class Model;
class Shader;

struct ShaderPool
{
    std::shared_ptr<Shader> phongShader;
    std::shared_ptr<Shader> phongMapsShader;

    ShaderPool(std::shared_ptr<Shader> phong, std::shared_ptr<Shader> phongMaps)
        : phongShader(std::move(phong)), phongMapsShader(std::move(phongMaps)) {}
};

class ModelImporter
{
public:
    static std::vector<Model> Import(const std::string& path, const ShaderPool& shaders);
    static std::vector<Model> Import(const std::string& path);
};


#endif //OPENGLENGINE_MODELIMPORTER_H