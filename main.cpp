#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Mesh.h"
#include "src/ModelImporter.h"
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



    auto phongShader = std::make_shared<Shader>("./shader/vertex.glsl", "./shader/phong.glsl");
    auto emeraldMat = Material();
    emeraldMat.SetShader(phongShader);
    emeraldMat.SetColorProperty("material.mainColor", glm::vec3(0.07568f,0.61424f,0.07568f));
    emeraldMat.SetColorProperty("material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
    emeraldMat.SetFloatProperty("material.shininess", 76.8f);

    auto sphere = PrimitiveFactory::CreateSphere();
    sphere.GetTransform().SetPosition(glm::vec3(1.7, 0.5f, -2.5));
    sphere.SetMaterial(emeraldMat);

    auto redPlasticMat = Material();
    redPlasticMat.SetShader(phongShader);
    redPlasticMat.SetColorProperty("material.mainColor",glm::vec3(0.5f, 0.0f, 0.0f));
    redPlasticMat.SetColorProperty("material.specular",glm::vec3(0.7f, 0.6f, 0.6f));
    redPlasticMat.SetFloatProperty("material.shininess", 32.0f);

    auto sphere2 = PrimitiveFactory::CreateSphere();
    sphere2.GetTransform().SetPosition(glm::vec3(-2.3f, 0.5f, 1.7f));
    sphere2.SetMaterial(redPlasticMat);

    auto plastic = Material();
    plastic.SetShader(phongShader);
    plastic.SetColorProperty("material.mainColor",glm::vec3(1.0f,1.0f,1.0f));
    plastic.SetColorProperty("material.specular",glm::vec3(0.70f,0.70f,0.70f));
    plastic.SetFloatProperty("material.shininess", 32.0f);

    auto cube1 = PrimitiveFactory::CreateCube();
    cube1.GetTransform().SetPosition(glm::vec3(0.0f, 2.0f, 4.0f));
    cube1.GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.2f));
    cube1.SetMaterial(plastic);

    auto cube2 = PrimitiveFactory::CreateCube();
    cube2.GetTransform().SetPosition(glm::vec3(0.0f, 2.0f, -4.0f));
    cube2.GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.2f));
    cube2.SetMaterial(plastic);

    auto cube3 = PrimitiveFactory::CreateCube();
    cube3.GetTransform().SetPosition(glm::vec3(4.0f, 2.0f, 0.0f));
    cube3.GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    cube3.GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.2f));
    cube3.SetMaterial(plastic);

    auto cube4 = PrimitiveFactory::CreateCube();
    cube4.GetTransform().SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));
    cube4.GetTransform().SetScale(glm::vec3(8.0f, 0.2f, 8.0f));
    cube4.SetMaterial(redPlasticMat);

    auto phongMapsShader = std::make_shared<Shader>("shader/vertex.glsl", "shader/phong_maps.glsl");
    auto diffuseTexture = std::make_shared<Texture>("image/container2_diffuse.png");
    auto specularTexture = std::make_shared<Texture>("image/container2_specular.png");
    auto container = Material();
    container.SetShader(phongMapsShader);
    container.SetTextureProperty("material.diffuse", diffuseTexture);
    container.SetTextureProperty("material.specular", specularTexture);
    container.SetFloatProperty("material.shininess", 28.0f);

    auto cube5 = PrimitiveFactory::CreateCube();
    cube5.GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    cube5.GetTransform().SetScale(glm::vec3(8.0f, 0.2f, 8.0f));
    cube5.SetMaterial(container);

    auto pointLight = Light();
    pointLight.SetType(LightType::Point);
    pointLight.SetLinear(0.22f);
    pointLight.SetQuadratic(0.20f);
    //pointLight.SetIntensity(0.5);
    pointLight.GetTransform().SetPosition(glm::vec3(0.0f, 2.7f, 0.0f));

    auto directionalLight = Light();
    directionalLight.SetType(LightType::Directional);
    directionalLight.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    directionalLight.SetIntensity(0.5f);
    directionalLight.setDirection(glm::vec3(0.2f, -1.0f, 0.3f));

    auto spotLight = Light();
    spotLight.SetType(LightType::Spot);
    spotLight.SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
    spotLight.setDirection(glm::vec3(0.5f, -1.0f, 0.0f));
    spotLight.GetTransform().SetPosition(glm::vec3(-0.5f, 3.0f, 0.0f));
    spotLight.SetCutoff(glm::cos(glm::radians(30.0f)));

    auto models = std::vector<Entity*>{&sphere, &sphere2, &cube1, &cube2, &cube3, &cube4, &cube5 };

    ShaderPool shaderPool(phongShader, phongMapsShader);

    auto mitsuba = ModelImporter::Import("model/mitsuba/mitsuba-sphere.obj", shaderPool);
    mitsuba.GetTransform().SetPosition(glm::vec3(0.7f, 0.0f, 0.0f));
    mitsuba.GetTransform().SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
    models.push_back(&mitsuba);

    // auto lego = ModelImporter::Import("model//lego_dragon.dae", shaderPool);
    // lego.GetTransform().SetPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
    // lego.GetTransform().SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
    // models.push_back(&lego);

    auto pointLights = std::vector<Light>{};
    pointLights.push_back(std::move(pointLight));
    auto spotLights = std::vector<Light>{};
    spotLights.push_back(std::move(spotLight));
    auto renderer = Renderer(models, std::move(directionalLight), std::move(pointLights), std::move(spotLights));
    Camera& camera = renderer.GetCamera();
    camera.GetTransform().SetPosition(glm::vec3(-4.0f, 2.0f, 0.0f));
    camera.SetForward(glm::vec3(1.0f, 0.0f, 0.0f));

    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, CameraMouseCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

