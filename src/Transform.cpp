//
// Created by louis on 10/26/25.
//

#include "Transform.h"

Transform::Transform()
{
    SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

glm::vec3 Transform::GetPosition() const
{
    return position;
}

void Transform::SetPosition(glm::vec3 position)
{
    this->position = position;
    positionMatrix = glm::translate(glm::mat4(1.0f), position);
    isDirty = true;
}

glm::vec3 Transform::GetRotation() const
{
    return rotation;
}

void Transform::SetRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
    const float x = rotation.x;
    const float y = rotation.y;
    const float z = rotation.z;

    rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
    isDirty = true;
}

glm::vec3 Transform::GetScale() const {
    return scale;
}

void Transform::SetScale(glm::vec3 scale)
{
    this->scale = scale;
    scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    isDirty = true;
}

glm::mat4 Transform::GetMatrix()
{
    if (isDirty)
    {
        UpdateMatrix();
        isDirty = false;
    }
    return modelMatrix;
}

void Transform::UpdateMatrix()
{
    modelMatrix = positionMatrix * rotationMatrix * scaleMatrix;
}

void Transform::UpdateMatrix(const glm::mat4 &parentMatrix)
{
    modelMatrix =  parentMatrix * modelMatrix;
}

