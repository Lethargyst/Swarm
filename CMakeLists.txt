cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME Swarm)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)
