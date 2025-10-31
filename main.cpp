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

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processCameraInputs(GLFWwindow* window, Camera& camera, float deltaTime)
{
    float speed = 2.5f * deltaTime;
    auto position = camera.GetTransform().GetPosition();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.SetPosition(position += speed * camera.GetForward());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.SetPosition(position -= speed * camera.GetForward());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.SetPosition(position -= speed * camera.GetRight());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.SetPosition(position += speed * camera.GetRight());
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.SetPosition(position += speed * camera.GetUp());
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.SetPosition(position -= speed * camera.GetUp());
}

float lastX = 400, lastY = 300;
float pitch = 0.0f, yaw = -90.0f;
bool firstMouse = true;
void CameraMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

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
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    auto vertex = Shader{"vertex", VERTEX ,"./shader/vertex.vert"};
    auto frag = Shader{"fragment", FRAGMENT, "./shader/fragment.frag"};
    vertex.Compile();
    frag.Compile();
    const auto program = ShaderProgram{&vertex, &frag};
    program.Link();

    const auto texture = Texture{"image/container.jpg"};
    auto cube = PrimitiveFactory::CreateCube();
    cube.SetTexture(texture);
    cube.GetTransform().SetPosition(glm::vec3(0, 0, 0));

    auto cube2 = PrimitiveFactory::CreateCube();
    cube2.SetTexture(texture);
    cube2.GetTransform().SetPosition(glm::vec3(1.5f, 2.0f, -2.0f));

    auto cube3 = PrimitiveFactory::CreateCube();
    cube3.SetTexture(texture);
    cube3.GetTransform().SetPosition(glm::vec3(1.0f, -2.0f, -4.0f));

    auto meshes = std::vector{cube, cube2, cube3};
    auto Shaders = std::vector{program};
    auto renderer = Renderer(meshes, program);
    Camera& camera = renderer.GetCamera();
    camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, CameraMouseCallback);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    // render loop
    while(!glfwWindowShouldClose(window))
    {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        processCameraInputs(window, camera, deltaTime);
        renderer.Update(currentFrame);
        renderer.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

