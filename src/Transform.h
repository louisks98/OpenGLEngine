//
// Created by louis on 10/26/25.
//

#ifndef OPENGLENGINE_TRANSFORM_H
#define OPENGLENGINE_TRANSFORM_H
#include <glm/gtc/matrix_transform.hpp>
#include "glm/vec3.hpp"


class Transform {
public:
    Transform();

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void SetScale(glm::vec3 scale);

    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetScale() const;
    glm::mat4 GetMatrix();
    void UpdateMatrix();
    void UpdateMatrix(const glm::mat4 &parentMatrix);

    bool IsDirty() const {return isDirty;};

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 positionMatrix;
    glm::mat4 rotationMatrix;
    glm::mat4 scaleMatrix;
    glm::mat4 modelMatrix;

    bool isDirty = false;
};


#endif //OPENGLENGINE_TRANSFORM_H