//
// Created by louis on 10/26/25.
//

#ifndef OPENGLENGINE_PRIMITIVEFACTORY_H
#define OPENGLENGINE_PRIMITIVEFACTORY_H
#include "Model.h"


class PrimitiveFactory
{
public:
    static Model CreateCube();
    static Model CreateSphere();
    static Model CreateCylinder();
    static Model CreateCone();
    static Model CreateIcoSphere();
};


#endif //OPENGLENGINE_PRIMITIVEFACTORY_H