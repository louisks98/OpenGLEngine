cmake_minimum_required(VERSION 4.0)
project(OpenGLEngine)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)


add_executable(OpenGLEngine main.cpp glad.c
        src/HelperFunctions.h
        src/rendering/Shader.cpp
        src/rendering/Shader.h
        src/rendering/Mesh.cpp
        src/rendering/Mesh.h
        include/stb_image.cpp
        src/rendering/Texture.cpp
        src/rendering/Texture.h
        src/rendering/Renderer.cpp
        src/rendering/Renderer.h
        src/scene/PrimitiveFactory.cpp
        src/scene/PrimitiveFactory.h
        src/entity/Model.cpp
        src/entity/Model.h
        src/entity/Transform.cpp
        src/entity/Transform.h
        src/entity/Camera.cpp
        src/entity/Camera.h
        src/entity/Entity.h
        src/rendering/Material.cpp
        src/rendering/Material.h
        src/entity/Light.h
        src/scene/ModelImporter.cpp
        src/scene/ModelImporter.h
        src/scene/Scene.cpp
        src/scene/Scene.h
        src/entity/Entity.cpp
        src/scene/ResourceManager.cpp
        src/scene/ResourceManager.h
        src/rendering/Skybox.cpp
        src/rendering/Skybox.h
        src/Window.cpp
        src/Window.h
        src/InputManager.cpp
        src/InputManager.h
        src/scene/SceneBuilder.cpp
        src/scene/SceneBuilder.h
)
target_include_directories(OpenGLEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(OpenGLEngine PRIVATE glfw OpenGL::GL assimp::assimp)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shader" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/image" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/model" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")