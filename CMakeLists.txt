cmake_minimum_required(VERSION 4.0)
project(OpenGLEngine)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW_PKG REQUIRED glfw3)

if (GLFW_PKG_FOUND)
    # If found via PkgConfig, set the necessary paths
    include_directories(${GLFW_PKG_INCLUDE_DIRS})
    set(GLFW_LIBRARIES ${GLFW_PKG_LIBRARIES})
else()
    # Fallback to the standard find_package if PkgConfig fails
    find_package(GLFW 3.3 REQUIRED)
    set(GLFW_LIBRARIES glfw) # Or 'glfw3' depending on the system
endif()


add_executable(OpenGLEngine main.cpp glad.c
        src/Shader.cpp
        src/Shader.h
        src/HelperFunctions.h
        src/ShaderProgram.cpp
        src/ShaderProgram.h)
target_include_directories(OpenGLEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(OpenGLEngine PRIVATE glfw OpenGL::GL X11 pthread dl)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shader" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")