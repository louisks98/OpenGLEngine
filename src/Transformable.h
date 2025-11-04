//
// Created by Louis on 10/30/2025.
//

#ifndef OPENGLENGINE_TRANSFORMABLE_H
#define OPENGLENGINE_TRANSFORMABLE_H
#include "Transform.h"


class Transformable {
    public:
    [[nodiscard]]
    Transform& GetTransform() { return transform;}

    [[nodiscard]]
    const Transform& GetTransform() const { return transform;}

protected:
    Transform transform;
};


#endif //OPENGLENGINE_TRANSFORMABLE_H