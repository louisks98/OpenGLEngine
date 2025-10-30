//
// Created by Louis on 10/30/2025.
//

#include "Camera.h"

Camera::Camera()
{
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    CalculateCameraAxes();
}

glm::mat4 Camera::GetViewMatrix() const
{
    if (viewMatrixDirty)
    {
        auto pos = this->transform.GetPosition();
        viewMatrix = glm::lookAt(pos, target, up);
        viewMatrixDirty = false;
    }
    return viewMatrix;
}

void Camera::SetPosition(glm::vec3 pos)
{
    this->transform.SetPosition(pos);
    CalculateCameraAxes();
    viewMatrixDirty = true;
}

void Camera::CalculateCameraAxes()
{
    forward = glm::normalize(this->transform.GetPosition() - target);
    right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));
    up = glm::cross(forward, right);
}

void Camera::SetTarget(glm::vec3 newTarget)
{
    target = newTarget;
    CalculateCameraAxes();
    viewMatrixDirty = true;
}

void Camera::LookAt(glm::vec3 pos, glm::vec3 newTarget)
{
    transform.SetPosition(pos);
    target = newTarget;
    CalculateCameraAxes();
    viewMatrixDirty = true;
}

