//
// Created by Louis on 12/4/2025.
//

#include "InputManager.h"

#include "entity/Camera.h"

InputManager::InputManager(GLFWwindow *window) : window(window){}

void InputManager::SetupCameraAndCursor(Camera *camera) const
{
    glfwSetWindowUserPointer(window, camera);
    glfwSetCursorPosCallback(window, CameraMouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::ProcessInput() const
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void InputManager::ProcessCameraInput(float deltaTime) const
{
    const float speed = 2.5f * deltaTime;
    auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    auto position = camera->GetTransform().GetPosition();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->SetPosition(position += speed * camera->GetForward());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->SetPosition(position -= speed * camera->GetForward());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->SetPosition(position -= speed * camera->GetRight());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->SetPosition(position += speed * camera->GetRight());
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->SetPosition(position += speed * camera->GetUp());
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->SetPosition(position -= speed * camera->GetUp());
}

double lastX = 400;
double lastY = 300;
double pitch = 0.0f;
double yaw = -90.0f;
bool firstMouse = true;
void InputManager::CameraMouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xOffset = xpos - lastX;
    double yOffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    constexpr float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw   += xOffset;
    pitch += yOffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    cam->SetForward(glm::normalize(direction));
}
