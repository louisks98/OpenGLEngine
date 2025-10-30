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

private:
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 forward;

    glm::mat4 mutable viewMatrix;
    bool mutable  viewMatrixDirty = true;

    void CalculateCameraAxes();
};


#endif //OPENGLENGINE_CAMERA_H