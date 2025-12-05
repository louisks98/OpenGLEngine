//
// Created by Louis on 10/30/2025.
//

#ifndef OPENGLENGINE_CAMERA_H
#define OPENGLENGINE_CAMERA_H
#include "Entity.h"
#include "glm/fwd.hpp"

class Camera : public Entity
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

    float GetFov() const {return fov;}
    void SetFov(const float fov) {this->fov = fov;}

    float GetAspect() const {return aspect;}
    void SetAspect(const float aspect) {this->aspect = aspect;}

    float GetNearPlane() const { return zNear;}
    void SetNearPlane(const float z_near) {zNear = z_near;}

    float GetFarPlane() const {return zFar;}
    void SetFarPlane(const float z_far) {zFar = z_far;}

    void LockToTarget(const bool value) { lockToTarget = value; };

private:
    float fov;
    float aspect;
    float zNear;
    float zFar;

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