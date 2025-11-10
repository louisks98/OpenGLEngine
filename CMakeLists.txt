cmake_minimum_required(VERSION 4.0)
project(OpenGLEngine)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)


add_executable(OpenGLEngine main.cpp glad.c
        src/HelperFunctions.h
        src/Shader.cpp
        src/Shader.h
        src/Mesh.cpp
        src/Mesh.h
        include/stb_image.cpp
        src/Texture.cpp
        src/Texture.h
        src/Renderer.cpp
        src/Renderer.h
        src/PrimitiveFactory.cpp
        src/PrimitiveFactory.h
        src/Model.cpp
        src/Model.h
        src/Transform.cpp
        src/Transform.h
        src/Camera.cpp
        src/Camera.h
        src/Entity.h
        src/Material.cpp
        src/Material.h
        src/Light.h
        src/ModelImporter.cpp
        src/ModelImporter.h
        src/Scene.cpp
        src/Scene.h
        src/Entity.cpp
        src/ResourceManager.cpp
        src/ResourceManager.h
)
target_include_directories(OpenGLEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(OpenGLEngine PRIVATE glfw OpenGL::GL assimp::assimp)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shader" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/image" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/model" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")