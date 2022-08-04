cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME Swarm)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDART 17)

add_executable( ${PROJECT_NAME} 
    src/main.cpp
    src/Math/Constants.h
    src/Math/Matrices.h
    src/Math/Matrices.cpp
    src/Math/Vectors.h
    src/Math/Vectors.cpp
    src/Objects/Objects.h
    src/Objects/Objects.cpp
    src/Objects/Swarm.h
    src/Objects/Swarm.cpp
    src/Render/ShaderProgram.h
    src/Render/ShaderProgram.cpp
    src/Render/Window.h
    src/Render/Window.cpp    
    src/Scene/Scene.h
    src/Scene/Scene.cpp
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)
