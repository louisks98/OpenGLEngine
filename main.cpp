#include <iostream>

#include "src/InputManager.h"
#include "src/Window.h"

#include "src/Mesh.h"
#include "src/ModelImporter.h"
#include "src/PrimitiveFactory.h"
#include "src/Renderer.h"
#include "src/SceneBuilder.h"
#include "src/Texture.h"

using namespace std;

int main() {
    Window window("OpenGL Engine", 800, 600);
    window.Initialize();

    InputManager inputManager(window.GetWindowContext());

    ResourceManager resourceManager;
    PrimitiveFactory primitiveFactory(&resourceManager);
    ModelImporter modelImporter(&resourceManager);
    Scene scene;
    SceneBuilder sceneBuilder(&scene, &primitiveFactory, &modelImporter);

    auto skybox = std::make_unique<Skybox>("image/skybox");
    scene.SetSkybox(std::move(skybox));

    auto diffuseTexture = make_shared<Texture>(Texture(TextureType::Simple, "./image/container2_diffuse.png")) ;
    auto specularTexture = make_shared<Texture>(Texture(TextureType::Simple, "./image/container2_specular.png"));
    auto windowTexture = make_shared<Texture>(Texture(TextureType::Simple, "./image/window.png"));

    auto phongMapShaderId = resourceManager.AddShader("./shader/vertex.glsl", "./shader/phong_maps.glsl","phong_maps");
    auto phongShaderId = resourceManager.AddShader("./shader/vertex.glsl", "./shader/phong.glsl", "phong");
    auto depthBufferDebugShaderId = resourceManager.AddShader("./shader/vertex.glsl", "./shader/DepthBufferDebug.glsl", "DepthBufferDebug");
    resourceManager.depthBufferDebugShaderId = depthBufferDebugShaderId;


    auto emeraldMatId = resourceManager.AddTranslucentMaterial(phongShaderId, glm::vec4(0.07568f,0.61424f,0.07568f, 0.2f), glm::vec4(0.633f, 0.727811f, 0.633f, 0.2f), 76.8f);
    Transform tf0;
    tf0.SetPosition(glm::vec3(-1, 0.6f, -2.5));
    sceneBuilder.AddSphere(emeraldMatId, tf0);

    auto redPlasticMatId = resourceManager.AddTranslucentMaterial(phongShaderId, glm::vec4(0.5f, 0.0f, 0.0f, 0.3f), glm::vec4(0.7f, 0.6f, 0.6f, 0.3f), 32.0f);
    Transform tf1;
    tf1.SetPosition(glm::vec3(-2.3f, 0.5f, 1.7f));
    sceneBuilder.AddSphere(redPlasticMatId, tf1);

    auto plasticMatId = resourceManager.AddMaterial(phongShaderId, glm::vec4(1.0f,1.0f,1.0f, 1.0f), glm::vec4(0.70f,0.70f,0.70f, 1.0f), 32.0f);
    Transform tf2(glm::vec3(0.0f, 2.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.0f, 4.0f, 0.2f));
    sceneBuilder.AddCube(plasticMatId, tf2);

    Transform tf3(glm::vec3(0.0f, 2.0f, -4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.0f, 4.0f, 0.2f));
    sceneBuilder.AddCube(plasticMatId, tf3);

    Transform tf4(glm::vec3(4.0f, 2.0f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(8.0f, 4.0f, 0.2f));
    sceneBuilder.AddCube(plasticMatId, tf4);

    Transform tf5(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.0f, 0.2f, 8.0f));
    sceneBuilder.AddCube(plasticMatId, tf5);

    auto containerMatId = resourceManager.AddTextureMaterial(phongMapShaderId, diffuseTexture, specularTexture, 28.0f);
    Transform tf6(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.0f, 0.2f, 8.0f));
    sceneBuilder.AddCube(containerMatId, tf6);

    auto windowMatId = resourceManager.AddTranslucentTextureMaterial(phongMapShaderId, windowTexture, windowTexture, 0.0f);
    Transform tf7(glm::vec3(-4.0f, 2.0f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(8.0f, 4.0f, 0.1f));
    sceneBuilder.AddCube(windowMatId, tf7);

    auto pointLight_ptr = make_unique<Light>();
    auto pointLight = pointLight_ptr.get();
    pointLight->SetType(LightType::Point);
    pointLight->SetLinear(0.22f);
    pointLight->SetQuadratic(0.20f);
    pointLight->SetIntensity(0.5);
    pointLight->GetTransform().SetPosition(glm::vec3(0.0f, 2.7f, 0.0f));
    pointLight->SetUpdateDelegate([](Entity* self, const float deltaTime) {
        const auto currentPos = self->GetTransform().GetPosition();
        const double currentTime = glfwGetTime();
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

    Transform suzanneTf(glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    sceneBuilder.AddModel("model/IridescenceSuzanne/glTF/IridescenceSuzanne.gltf", suzanneTf);

    Transform duckTf(glm::vec3(0.5, 0.0f, -2.5), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
    sceneBuilder.AddModel("model/Duck/glTF/Duck.gltf", duckTf);

    auto renderer = Renderer(&scene, &resourceManager);
    Camera& camera = scene.GetCamera();
    camera.GetTransform().SetPosition(glm::vec3(-4.0f, 2.0f, 0.0f));
    camera.SetForward(glm::vec3(1.0f, 0.0f, 0.0f));

    inputManager.SetupCameraAndCursor(&camera);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    // render loop
    while(!glfwWindowShouldClose(window.GetWindowContext()))
    {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputManager.ProcessInput();
        inputManager.ProcessCameraInput(deltaTime);
        renderer.Update(deltaTime);
        renderer.Render();

        glfwSwapBuffers(window.GetWindowContext());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

