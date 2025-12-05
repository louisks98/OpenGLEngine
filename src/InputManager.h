//
// Created by Louis on 12/4/2025.
//

#ifndef OPENGLENGINE_INPUTMANAGER_H
#define OPENGLENGINE_INPUTMANAGER_H
#include "Camera.h"
#include "Window.h"


class InputManager
{
public:
    InputManager(GLFWwindow* window);

    void SetupCameraAndCursor(Camera* camera) const;
    void ProcessInput() const;
    void ProcessCameraInput(float deltaTime) const;
private:
    GLFWwindow* window;

    static void CameraMouseCallback(GLFWwindow* window, double xpos, double ypos);
};


#endif //OPENGLENGINE_INPUTMANAGER_H