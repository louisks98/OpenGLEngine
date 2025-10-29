#include <cmath>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Mesh.h"
#include "src/PrimitiveFactory.h"
#include "src/Renderer.h"
#include "src/Shader.h"
#include "src/ShaderProgram.h"
#include "src/Texture.h"

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
        -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
   };
    uint32_t indices1[] = {
        0, 1, 2,
    };

    const auto triangle1 = Mesh{vertices1, 24, indices1, 3};
    const auto texture = Texture{"image/container.jpg"};
    auto triangle = Model{triangle1};
    triangle.SetTexture(texture);

    auto cube = PrimitiveFactory::CreateCube();
    cube.SetTexture(texture);
    Transform transform = cube.GetTransform();
    transform.SetPosition(glm::vec3(-1.0f, 2.0f, -10.0f));

    auto cube2 = PrimitiveFactory::CreateCube();
    cube2.SetTexture(texture);
    transform = cube2.GetTransform();
    transform.SetPosition(glm::vec3(4.0f, -3.0f, 0.0f));

    auto cube3 = PrimitiveFactory::CreateCube();
    cube3.SetTexture(texture);
    transform = cube3.GetTransform();
    transform.SetPosition(glm::vec3(7.0f, 9.0f, -4.0f));

    auto meshes = std::vector{cube, cube2, cube3};
    auto Shaders = std::vector{program};
    auto renderer = Renderer(meshes, program);

    // render loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        renderer.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

