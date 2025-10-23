#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Primitive.h"
#include "src/Shader.h"
#include "src/ShaderProgram.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Window setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    auto vertex = Shader{"vertex", VERTEX ,"./shader/vertex.vert"};
    auto frag = Shader{"fragment", FRAGMENT, "./shader/fragment.frag"};

    vertex.Compile();
    frag.Compile();

    const auto program = ShaderProgram{&vertex, &frag};
    program.Link();

    float vertices1[] = {
        0.0f,  0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        1.0f, 0.0f, 0.0f,
   };

    uint32_t indices1[] = {
        0, 1, 2,
    };

    const auto triangle1 = Primitive{vertices1, sizeof(vertices1), indices1, sizeof(indices1)};

    float vertices2[] = {
        0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -1.0f, 0.0f, 0.0f,
   };

    const auto triangle2 = Primitive{vertices2, sizeof(vertices2), indices1, sizeof(indices1)};

    // render loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use draw triangle
        program.Use();
        triangle1.Draw();
        triangle2.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

