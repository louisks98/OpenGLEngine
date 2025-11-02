//
// Created by Louis on 11/1/2025.
//

#ifndef OPENGLENGINE_LIGHT_H
#define OPENGLENGINE_LIGHT_H
#include "Transformable.h"


class Light : public Transformable
{
public:
    [[nodiscard]]
    glm::vec3 GetColor() const {return color;};
    void SetColor(const glm::vec3 newColor) {color = newColor;};

    [[nodiscard]]
    float GetIntensity() const {return intensity;};
    void SetIntensity(const float newIntensity) {intensity = newIntensity;};

private:
    glm::vec3 color = {1.0f, 1.0f, 1.0f};
    float intensity = 1;
};


#endif //OPENGLENGINE_LIGHT_H