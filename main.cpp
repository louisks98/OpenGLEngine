#include <iostream>
#include "src/Window.h"

#include "src/Mesh.h"
#include "src/ModelImporter.h"
#include "src/PrimitiveFactory.h"
#include "src/Renderer.h"
#include "src/Shader.h"
#include "src/Texture.h"

using namespace std;

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
    Window window = Window("OpenGL Engine", 800, 600);
    window.Initialize();

    ResourceManager resourceManager;
    PrimitiveFactory primitiveFactory(&resourceManager);
    ModelImporter modelImporter(&resourceManager);
    Scene scene;

    auto skybox = std::make_unique<Skybox>("image/skybox");
    scene.SetSkybox(std::move(skybox));

    auto phongMapsShader = Shader("shader/vertex.glsl", "shader/phong_maps.glsl","phong_maps");
    auto diffuseTexture = make_shared<Texture>(Texture(TextureType::Simple, "image/container2_diffuse.png")) ;
    auto specularTexture = make_shared<Texture>(Texture(TextureType::Simple, "image/container2_specular.png"));
    auto windowTexture = make_shared<Texture>(Texture(TextureType::Simple, "image/window.png"));

    auto phongShader = Shader("./shader/vertex.glsl", "./shader/phong.glsl", "phong");
    auto depthBufferDebugShader = Shader("./shader/vertex.glsl", "./shader/DepthBufferDebug.glsl", "DepthBufferDebug");

    auto phongMapShaderId = resourceManager.AddShader(std::move(phongMapsShader));
    auto phongShaderId = resourceManager.AddShader(std::move(phongShader));
    auto depthBufferDebugShaderId = resourceManager.AddShader(std::move(depthBufferDebugShader));
    resourceManager.depthBufferDebugShaderId = depthBufferDebugShaderId;

    auto emeraldMat = Material();
    emeraldMat.SetShader(phongShaderId);
    emeraldMat.SetIntProperty("material.type", MaterialType::Translucent);
    emeraldMat.SetColorProperty("material.mainColor", glm::vec4(0.07568f,0.61424f,0.07568f, 0.2f));
    emeraldMat.SetColorProperty("material.specular", glm::vec4(0.633f, 0.727811f, 0.633f, 0.2f));
    emeraldMat.SetFloatProperty("material.shininess", 76.8f);

    auto emeraldMatId = resourceManager.AddMaterial(std::move(emeraldMat));

    auto sphereMeshId = primitiveFactory.CreateSphere();
    auto sphereModel = make_unique<Model>(Model(sphereMeshId, emeraldMatId));
    sphereModel->GetTransform().SetPosition(glm::vec3(-1, 0.6f, -2.5));
    scene.AddEntity(std::move(sphereModel));

    auto redPlasticMat = Material();
    redPlasticMat.SetShader(phongShaderId);
    redPlasticMat.SetIntProperty("material.type", MaterialType::Translucent);
    redPlasticMat.SetColorProperty("material.mainColor",glm::vec4(0.5f, 0.0f, 0.0f, 0.3f));
    redPlasticMat.SetColorProperty("material.specular",glm::vec4(0.7f, 0.6f, 0.6f, 0.3f));
    redPlasticMat.SetFloatProperty("material.shininess", 32.0f);

    auto redPlasticMatId = resourceManager.AddMaterial(std::move(redPlasticMat));


    auto sphere2MeshId = primitiveFactory.CreateSphere();
    auto sphere2Model = make_unique<Model>(Model(sphere2MeshId, redPlasticMatId));
    sphere2Model->GetTransform().SetPosition(glm::vec3(-2.3f, 0.5f, 1.7f));
    scene.AddEntity(std::move(sphere2Model));

    auto plastic = Material();
    plastic.SetShader(phongShaderId);
    plastic.SetColorProperty("material.mainColor",glm::vec4(1.0f,1.0f,1.0f, 1.0f));
    plastic.SetColorProperty("material.specular",glm::vec4(0.70f,0.70f,0.70f, 1.0f));
    plastic.SetFloatProperty("material.shininess", 32.0f);

    auto plasticMatId = resourceManager.AddMaterial(std::move(plastic));

    auto cube1MeshId = primitiveFactory.CreateCube();
    auto cube1Model = make_unique<Model>(Model(cube1MeshId, plasticMatId));
    cube1Model->GetTransform().SetPosition(glm::vec3(0.0f, 2.0f, 4.0f));
    cube1Model->GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.2f));
    scene.AddEntity(std::move(cube1Model));

    auto cube2MeshId = primitiveFactory.CreateCube();
    auto cube2Model = make_unique<Model>(Model(cube2MeshId, plasticMatId));
    cube2Model->GetTransform().SetPosition(glm::vec3(0.0f, 2.0f, -4.0f));
    cube2Model->GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.2f));
    scene.AddEntity(std::move(cube2Model));

    auto cube3MeshId = primitiveFactory.CreateCube();
    auto cube3Model = make_unique<Model>(Model(cube3MeshId, plasticMatId));
    cube3Model->GetTransform().SetPosition(glm::vec3(4.0f, 2.0f, 0.0f));
    cube3Model->GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    cube3Model->GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.2f));
    scene.AddEntity(std::move(cube3Model));

    auto cube4MeshId = primitiveFactory.CreateCube();
    auto cube4Model = make_unique<Model>(Model(cube4MeshId, redPlasticMatId));
    cube4Model->GetTransform().SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));
    cube4Model->GetTransform().SetScale(glm::vec3(8.0f, 0.2f, 8.0f));
    scene.AddEntity(std::move(cube4Model));

    auto container = Material();
    container.SetShader(phongMapShaderId);
    container.SetTextureProperty("material.diffuse", diffuseTexture);
    container.SetTextureProperty("material.specular", specularTexture);
    container.SetFloatProperty("material.shininess", 28.0f);

    auto containerMatId = resourceManager.AddMaterial(std::move(container));

    auto cube5MeshId = primitiveFactory.CreateCube();
    auto cube5Model = make_unique<Model>(Model(cube5MeshId, containerMatId));
    cube5Model->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    cube5Model->GetTransform().SetScale(glm::vec3(8.0f, 0.2f, 8.0f));
    scene.AddEntity(std::move(cube5Model));

    auto windowMat = Material();
    windowMat.SetShader(phongMapShaderId);
    windowMat.SetIntProperty("material.type", MaterialType::Translucent);
    windowMat.SetTextureProperty("material.diffuse", windowTexture);
    windowMat.SetTextureProperty("material.specular", windowTexture);
    plastic.SetFloatProperty("material.shininess", 0.0f);

    auto windowMatId = resourceManager.AddMaterial(std::move(windowMat));

    auto cubeWindowMeshID = primitiveFactory.CreateCube();
    auto cubeWindowModel = make_unique<Model>(Model(cubeWindowMeshID, windowMatId));
    cubeWindowModel->GetTransform().SetPosition(glm::vec3(-4.0f, 2.0f, 0.0f));
    cubeWindowModel->GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    cubeWindowModel->GetTransform().SetScale(glm::vec3(8.0f, 4.0f, 0.1f));
    scene.AddEntity(std::move(cubeWindowModel));

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
    directionalLight->SetType(LightType::Directional);
    directionalLight->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    directionalLight->SetIntensity(0.5f);
    directionalLight->setDirection(glm::vec3(0.2f, -1.0f, 0.3f));
    scene.AddEntity(std::move(directionalLight));

    auto spotLight = make_unique<Light>();
    spotLight->SetType(LightType::Spot);
    spotLight->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
    spotLight->setDirection(glm::vec3(0.5f, -1.0f, 0.0f));
    spotLight->GetTransform().SetPosition(glm::vec3(-0.5f, 3.0f, 0.0f));
    spotLight->SetCutoff(glm::cos(glm::radians(30.0f)));
    scene.AddEntity(std::move(spotLight));

    auto suzanne = modelImporter.Import("model/IridescenceSuzanne/glTF/IridescenceSuzanne.gltf");
    suzanne.GetTransform().SetPosition(glm::vec3(0.0f, 1.5f, 0.0f));
    suzanne.GetTransform().SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
    scene.AddEntity(make_unique<Entity>(std::move(suzanne)));

    auto duck = modelImporter.Import("model/Duck/glTF/Duck.gltf");
    duck.GetTransform().SetPosition(glm::vec3(0.5, 0.0f, -2.5));
    duck.GetTransform().SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    duck.GetTransform().SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
    scene.AddEntity(make_unique<Entity>(std::move(duck)));

    auto renderer = Renderer(&scene, &resourceManager);
    Camera& camera = scene.GetCamera();
    camera.GetTransform().SetPosition(glm::vec3(-4.0f, 2.0f, 0.0f));
    camera.SetForward(glm::vec3(1.0f, 0.0f, 0.0f));

    glfwSetWindowUserPointer(window.GetWindowContext(), &camera);
    glfwSetCursorPosCallback(window.GetWindowContext(), CameraMouseCallback);
    glfwSetInputMode(window.GetWindowContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    // render loop
    while(!glfwWindowShouldClose(window.GetWindowContext()))
    {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window.GetWindowContext());
        processCameraInputs(window.GetWindowContext(), camera, deltaTime);
        renderer.Update(deltaTime);
        renderer.Render();

        glfwSwapBuffers(window.GetWindowContext());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

