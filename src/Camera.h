//
// Created by Louis on 10/30/2025.
//

#ifndef OPENGLENGINE_CAMERA_H
#define OPENGLENGINE_CAMERA_H
#include "Transformable.h"


class Camera : public Transformable
{
public:
    Camera();

    glm::mat4 GetViewMatrix() const;
    void SetPosition(glm::vec3 pos);
    void SetTarget(glm::vec3 pos);
    void LookAt(glm::vec3 pos, glm::vec3 target);
    glm::vec3 GetForward() const { return forward; };
    void SetForward(glm::vec3 forward);
    glm::vec3 GetRight() const { return  right; };
    glm::vec3 GetUp() const { return  up; };

    void LockToTarget(const bool value) { lockToTarget = value; };

private:
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 forward;

    glm::mat4 mutable viewMatrix;
    bool mutable  viewMatrixDirty = true;

    bool lockToTarget = false;

    void CalculateCameraAxes();
};


#endif //OPENGLENGINE_CAMERA_H