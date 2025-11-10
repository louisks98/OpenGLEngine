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

    ResourceManager resourceManager;
    PrimitiveFactory primitiveFactory(&resourceManager);
    ModelImporter modelImporter(&resourceManager);
    Scene scene;

    auto phongMapsShader = Shader("shader/vertex.glsl", "shader/phong_maps.glsl","phong_maps");
    auto diffuseTexture = make_shared<Texture>(Texture("image/container2_diffuse.png")) ;
    auto specularTexture = make_shared<Texture>(Texture("image/container2_specular.png"));

    auto phongShader = Shader("./shader/vertex.glsl", "./shader/phong.glsl", "phong");

    auto phongMapShaderId = resourceManager.AddShader(std::move(phongMapsShader));
    auto phongShaderId = resourceManager.AddShader(std::move(phongShader));

    auto emeraldMat = Material();
    emeraldMat.SetShader(phongShaderId);
    emeraldMat.SetColorProperty("material.mainColor", glm::vec3(0.07568f,0.61424f,0.07568f));
    emeraldMat.SetColorProperty("material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
    emeraldMat.SetFloatProperty("material.shininess", 76.8f);

    auto emeraldMatId = resourceManager.AddMaterial(std::move(emeraldMat));

    auto sphereMeshId = primitiveFactory.CreateSphere();
    auto sphereModel = make_unique<Model>(Model(sphereMeshId, emeraldMatId));
    sphereModel.get()->GetTransform().SetPosition(glm::vec3(1.7, 0.5f, -2.5));
    scene.AddEntity(std::move(sphereModel));

    auto redPlasticMat = Material();
    redPlasticMat.SetShader(phongShaderId);
    redPlasticMat.SetColorProperty("material.mainColor",glm::vec3(0.5f, 0.0f, 0.0f));
    redPlasticMat.SetColorProperty("material.specular",glm::vec3(0.7f, 0.6f, 0.6f));
    redPlasticMat.SetFloatProperty("material.shininess", 32.0f);

    auto redPlasticMatId = resourceManager.AddMaterial(std::move(redPlasticMat));


    auto sphere2MeshId = primitiveFactory.CreateSphere();
    auto sphere2Model = make_unique<Model>(Model(sphere2MeshId, redPlasticMatId));
    sphere2Model.get()->GetTransform().SetPosition(glm::vec3(-2.3f, 0.5f, 1.7f));
    scene.AddEntity(std::move(sphere2Model));

    auto plastic = Material();
    plastic.SetShader(phongShaderId);
    plastic.SetColorProperty("material.mainColor",glm::vec3(1.0f,1.0f,1.0f));
    plastic.SetColorProperty("material.specular",glm::vec3(0.70f,0.70f,0.70f));
    plastic.SetFloatProperty("material.shininess", 32.0f);

    auto plasticMatId = resourceManager.AddMaterial(std::move(plastic));

    auto cube1MeshId = primitiveFactory.CreateCube();
    auto cube1Model = make_unique<Model>(Model(cube1MeshId, plasticMatId));
    cube1Model.get()->GetTransform().SetPosition(glm::vec3(0.0f, 2.0f, 4.0f));
    cube1Model.get()->GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.2f));
    scene.AddEntity(std::move(cube1Model));

    auto cube2MeshId = primitiveFactory.CreateCube();
    auto cube2Model = make_unique<Model>(Model(cube2MeshId, plasticMatId));
    cube2Model.get()->GetTransform().SetPosition(glm::vec3(0.0f, 2.0f, -4.0f));
    cube2Model.get()->GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.2f));
    scene.AddEntity(std::move(cube2Model));

    auto cube3MeshId = primitiveFactory.CreateCube();
    auto cube3Model = make_unique<Model>(Model(cube3MeshId, plasticMatId));
    cube3Model.get()->GetTransform().SetPosition(glm::vec3(4.0f, 2.0f, 0.0f));
    cube3Model.get()->GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    cube3Model.get()->GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.2f));
    scene.AddEntity(std::move(cube3Model));

    auto cube4MeshId = primitiveFactory.CreateCube();
    auto cube4Model = make_unique<Model>(Model(cube4MeshId, redPlasticMatId));
    cube4Model.get()->GetTransform().SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));
    cube4Model.get()->GetTransform().SetScale(glm::vec3(8.0f, 0.2f, 8.0f));
    scene.AddEntity(std::move(cube4Model));

    auto container = Material();
    container.SetShader(phongMapShaderId);
    container.SetTextureProperty("material.diffuse", diffuseTexture);
    container.SetTextureProperty("material.specular", specularTexture);
    container.SetFloatProperty("material.shininess", 28.0f);

    auto containerMatId = resourceManager.AddMaterial(std::move(container));

    auto cube5MeshId = primitiveFactory.CreateCube();
    auto cube5Model = make_unique<Model>(Model(cube5MeshId, containerMatId));
    cube5Model.get()->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    cube5Model.get()->GetTransform().SetScale(glm::vec3(8.0f, 0.2f, 8.0f));
    scene.AddEntity(std::move(cube5Model));

    auto pointLight_ptr = make_unique<Light>();
    auto pointLight = pointLight_ptr.get();
    pointLight->SetType(LightType::Point);
    pointLight->SetLinear(0.22f);
    pointLight->SetQuadratic(0.20f);
    //pointLight.SetIntensity(0.5);
    pointLight->GetTransform().SetPosition(glm::vec3(0.0f, 2.7f, 0.0f));
    pointLight->SetUpdateDelegate([](Entity* self, const float deltaTime) {
        const auto currentPos = self->GetTransform().GetPosition();
        const float currentTime = glfwGetTime();
        self->GetTransform().SetPosition(glm::vec3(glm::cos(currentTime) * 2.5, currentPos.y, glm::sin(currentTime) * 2.5));
    });
    scene.AddEntity(std::move(pointLight_ptr));

    auto directionalLight = make_unique<Light>();
    directionalLight.get()->SetType(LightType::Directional);
    directionalLight.get()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    directionalLight.get()->SetIntensity(0.5f);
    directionalLight.get()->setDirection(glm::vec3(0.2f, -1.0f, 0.3f));
    scene.AddEntity(std::move(directionalLight));

    auto spotLight = make_unique<Light>();
    spotLight.get()->SetType(LightType::Spot);
    spotLight.get()->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
    spotLight.get()->setDirection(glm::vec3(0.5f, -1.0f, 0.0f));
    spotLight.get()->GetTransform().SetPosition(glm::vec3(-0.5f, 3.0f, 0.0f));
    spotLight.get()->SetCutoff(glm::cos(glm::radians(30.0f)));
    scene.AddEntity(std::move(spotLight));

    auto mitsuba = modelImporter.Import("model/mitsuba/mitsuba-sphere.obj");
    mitsuba.GetTransform().SetPosition(glm::vec3(0.7f, 0.0f, 0.0f));
    mitsuba.GetTransform().SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
    scene.AddEntity(make_unique<Entity>(std::move(mitsuba)));

    // auto lego = ModelImporter::Import("model//lego_dragon.dae", shaderPool);
    // lego.GetTransform().SetPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
    // lego.GetTransform().SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
    // models.push_back(&lego);

    auto renderer = Renderer(&scene, &resourceManager);
    Camera& camera = scene.GetCamera();
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
        renderer.Update(deltaTime);
        renderer.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

