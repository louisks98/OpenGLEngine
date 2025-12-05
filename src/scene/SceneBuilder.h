//
// Created by Louis on 12/4/2025.
//

#ifndef OPENGLENGINE_SCENEBUILDER_H
#define OPENGLENGINE_SCENEBUILDER_H
#include "ModelImporter.h"
#include "PrimitiveFactory.h"

class SceneBuilder
{
public:
    SceneBuilder(Scene* scene, PrimitiveFactory* primitiveFactory, ModelImporter* modelImporter);

    void AddCube(uint32_t matId, const Transform &transform) const;
    void AddSphere(uint32_t matId, const Transform &transform) const;
    void AddModel(const std::string &modelPath, const Transform &transform) const;

private:
    Scene* scene;
    PrimitiveFactory* primitiveFactory;
    ModelImporter* modelImporter;
};


#endif //OPENGLENGINE_SCENEBUILDER_H