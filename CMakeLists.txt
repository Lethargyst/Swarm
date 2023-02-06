cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME Swarm)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDART 17)

add_executable( ${PROJECT_NAME} 
    src/main.cpp
    src/precompiled.hpp
    
    src/Config/Config.hpp
    src/Config/Config.cpp
    
    src/Json/json.hpp

    src/Math/Algebra.hpp
    src/Math/Algebra.cpp
    
    src/Math/Matrices.hpp
    src/Math/Matrices.cpp
    
    src/Math/Random.hpp

    src/Math/Vectors.hpp
    src/Math/Vectors.cpp
    
    src/Objects/CollisionManager.hpp
    src/Objects/CollisionManager.cpp
    
    src/Objects/Geometry2D.hpp
    src/Objects/Geometry2D.cpp

    src/Objects/Objects.hpp
    src/Objects/Objects.cpp
    
    src/Render/ShaderProgram.hpp
    src/Render/ShaderProgram.cpp
    
    src/Render/Window.hpp
    src/Render/Window.cpp    

    src/Scene/QuadTree.hpp
    src/Scene/QuadTree.tpp

    src/Scene/Scene.hpp
    src/Scene/Scene.cpp

    src/Swarm/Swarm.hpp
    src/Swarm/Swarm.cpp
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

