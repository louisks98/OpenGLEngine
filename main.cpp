#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Mesh.h"
#include "src/PrimitiveFactory.h"
#include "src/Renderer.h"
#include "src/Shader.h"
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


    auto shader = std::make_shared<Shader>("./shader/vertex.glsl", "./shader/phong.glsl");
    auto emeraldMat = Material();
    emeraldMat.SetShader(shader);
    emeraldMat.SetColorProperty("material.mainColor", glm::vec3(0.07568f,0.61424f,0.07568f));
    emeraldMat.SetColorProperty("material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
    emeraldMat.SetFloatProperty("material.shininess", 76.8f);

    auto sphere = PrimitiveFactory::CreateSphere();
    sphere.GetTransform().SetPosition(glm::vec3(0, 0, 4));
    sphere.SetMaterial(emeraldMat);

    auto bronzeMat = Material();
    bronzeMat.SetShader(shader);
    bronzeMat.SetColorProperty("material.mainColor",glm::vec3(0.714f,0.4284f,0.18144f));
    bronzeMat.SetColorProperty("material.specular",glm::vec3(0.393548f,0.4284f,0.271906f));
    bronzeMat.SetFloatProperty("material.shininess", 25.6f);

    auto cube2 = PrimitiveFactory::CreateCube();
    cube2.GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, -4.0f));
    cube2.SetMaterial(bronzeMat);

    auto redPlasticMat = Material();
    redPlasticMat.SetShader(shader);
    redPlasticMat.SetColorProperty("material.mainColor",glm::vec3(0.5f, 0.0f, 0.0f));
    redPlasticMat.SetColorProperty("material.specular",glm::vec3(0.7f, 0.6f, 0.6f));
    redPlasticMat.SetFloatProperty("material.shininess", 32.0f);

    auto sphere2 = PrimitiveFactory::CreateSphere();
    sphere2.GetTransform().SetPosition(glm::vec3(4.0f, 0.0f, 0.0f));
    sphere2.SetMaterial(redPlasticMat);

    auto mapShader = std::make_shared<Shader>("shader/vertex.glsl", "shader/phong_maps.glsl");
    auto diffuseTexture = std::make_shared<Texture>("image/container2_diffuse.png");
    auto specularTexture = std::make_shared<Texture>("image/container2_specular.png");
    auto container = Material();
    container.SetShader(mapShader);
    container.SetTextureProperty("material.diffuse", diffuseTexture);
    container.SetTextureProperty("material.specular", specularTexture);
    container.SetFloatProperty("material.shininess", 28.0f);

    auto cube4 = PrimitiveFactory::CreateCube();
    cube4.GetTransform().SetPosition(glm::vec3(-4.0f, 0.0f, 0.0f));
    cube4.SetMaterial(container);

    auto pointLight = Light();
    pointLight.SetType(LightType::Point);
    pointLight.SetLinear(0.09f);
    pointLight.SetQuadratic(0.032f);
    pointLight.GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    auto directionalLight = Light();
    directionalLight.SetType(LightType::Directional);
    directionalLight.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    directionalLight.setDirection(glm::vec3(0.2f, -1.0f, 0.3f));

    auto spotLight = Light();
    spotLight.SetType(LightType::Spot);
    spotLight.SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
    spotLight.setDirection(glm::vec3(0.5f, -1.0f, 0.0f));
    spotLight.GetTransform().SetPosition(glm::vec3(4.0f, 1.5f, 0.0f));
    spotLight.SetCutoff(glm::cos(glm::radians(20.0f)));


    auto models = std::vector{sphere, cube2, sphere2, cube4};
    auto pointLights = std::vector{pointLight};
    auto spotLights = std::vector{spotLight};
    auto renderer = Renderer(models, directionalLight, pointLights, spotLights);
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
        renderer.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

