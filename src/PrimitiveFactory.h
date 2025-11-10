//
// Created by louis on 10/26/25.
//

#ifndef OPENGLENGINE_PRIMITIVEFACTORY_H
#define OPENGLENGINE_PRIMITIVEFACTORY_H
#include "Model.h"
#include "ResourceManager.h"


class PrimitiveFactory
{
public:
    explicit PrimitiveFactory(ResourceManager *resourceManager) : resourceManager(resourceManager) {}

    uint32_t CreateCube() const;
    uint32_t CreateSphere() const;
    static Model CreateCylinder();
    static Model CreateCone();
    static Model CreateIcoSphere();

private:
    ResourceManager* resourceManager;
};


#endif //OPENGLENGINE_PRIMITIVEFACTORY_H