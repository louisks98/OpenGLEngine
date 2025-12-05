//
// Created by Louis on 12/4/2025.
//

#ifndef OPENGLENGINE_WINDOW_H
#define OPENGLENGINE_WINDOW_H
#include <string>
#include <GLFW/glfw3.h>


class Window {
public:
    Window(std::string t, int w, int h);
    void Initialize();

    GLFWwindow* GetWindowContext() const;
private:
    GLFWwindow* window;
    std::string title;
    int width;
    int height;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};


#endif //OPENGLENGINE_WINDOW_H