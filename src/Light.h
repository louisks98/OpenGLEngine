//
// Created by Louis on 11/1/2025.
//

#ifndef OPENGLENGINE_LIGHT_H
#define OPENGLENGINE_LIGHT_H
#include "Transformable.h"

enum LightType
{
    Directional = 0,
    Point = 1,
    Spot = 2,
};

class Light : public Transformable
{
public:
    [[nodiscard]]
    LightType GetType() const {return type;}
    void SetType(const LightType newType) {this->type = newType;}

    [[nodiscard]]
    glm::vec3 GetColor() const {return color;};
    void SetColor(const glm::vec3 newColor) {color = newColor;};

    [[nodiscard]]
    float GetIntensity() const {return intensity;};
    void SetIntensity(const float newIntensity) {intensity = newIntensity;};

    [[nodiscard]]
    glm::vec3 GetDirection() const {return direction;}
    void setDirection(const glm::vec3 &newDirection) {this->direction = newDirection;}

    [[nodiscard]]
    static float GetConstant() {return 1.0f;};

    [[nodiscard]]
    float GetLinear() const {return linear;}
    void SetLinear(const float newLinear) {this->linear = newLinear;}

    [[nodiscard]]
    float GetQuadratic() const {return quadratic;}
    void SetQuadratic(const float newQuadratic) {this->quadratic = newQuadratic;}

    [[nodiscard]]
    float GetCutoff() const {return cutOff;}
    void SetCutoff(const float newCutoff) {cutOff = newCutoff;}

private:
    // General
    LightType type = Point;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};
    float intensity = 1;

    // Directional, Spot
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};

    // Point
    float linear = 0.0f;
    float quadratic = 0.0f;

    // Spot
    float cutOff = 0.0f;
};


#endif //OPENGLENGINE_LIGHT_H