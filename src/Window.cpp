//
// Created by Louis on 12/4/2025.
//

#include <iostream>
#include <glad/glad.h>
#include "Window.h"

Window::Window(std::string t, int w, int h)
{
    title = t;
    width = w;
    height = h;
    window = nullptr;
}

void Window::Initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
}

GLFWwindow * Window::GetWindowContext() const
{
    return window;
}
