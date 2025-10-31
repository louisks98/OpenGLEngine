//
// Created by Louis on 10/30/2025.
//

#include "Camera.h"

Camera::Camera()
{
    target = glm::vec3(NAN, NAN, NAN);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const
{
    if (viewMatrixDirty)
    {
        auto pos = this->transform.GetPosition();
        viewMatrix = glm::lookAt(pos, pos + forward, up);
        viewMatrixDirty = false;
    }
    return viewMatrix;
}

void Camera::SetPosition(glm::vec3 pos)
{
    this->transform.SetPosition(pos);
    viewMatrixDirty = true;
}

void Camera::CalculateCameraAxes()
{
    if (!lockToTarget)
        return;

    const auto fwd = target - this->transform.GetPosition();
    if (glm::length(fwd) == 0)
        forward = glm::vec3(0.0f, 0.0f, -1.0f);
    else
        forward = glm::normalize(fwd);

    const auto r = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
    if (glm::length(r) == 0)
        right = glm::vec3(1.0f, 0.0f, 0.0f);
    else
        right = glm::normalize(r);

    up = glm::cross(right, forward);
}

void Camera::SetTarget(glm::vec3 newTarget)
{
    target = newTarget;
    CalculateCameraAxes();
    viewMatrixDirty = true;
    lockToTarget = true;
}

void Camera::LookAt(glm::vec3 pos, glm::vec3 newTarget)
{
    transform.SetPosition(pos);
    target = newTarget;
    CalculateCameraAxes();
    viewMatrixDirty = true;
    lockToTarget = true;
}

void Camera::SetForward(glm::vec3 fwd)
{
    forward = fwd;
    const auto r = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
    if (glm::length(r) == 0)
        right = glm::vec3(1.0f, 0.0f, 0.0f);
    else
        right = glm::normalize(r);

    up = glm::cross(right, forward);
    viewMatrixDirty = true;
}


