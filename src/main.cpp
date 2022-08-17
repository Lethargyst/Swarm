#include <iostream>
#include <math.h>
#include "windows.h"
#include "Math/Vectors.h"
#include "Scene/Scene.h"


int main(int argc, char* argv[])
{
    Renderer::initGLFW(4, 6);
    Window window(800, 600, "Test");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::string vertexShaderSource, fragmentShaderSource, geometryShaderSource;
    Renderer::loadSource("../src/Render/Shaders/VertexShader.vert", vertexShaderSource);
    Renderer::loadSource("../src/Render/Shaders/FragmentShader.frag", fragmentShaderSource);
    Renderer::loadSource("../src/Render/Shaders/GeometryShader.geom", geometryShaderSource);
    Renderer::ShaderProgram shader(vertexShaderSource, fragmentShaderSource, geometryShaderSource);

    Scene& scene = Scene::initialize(&window, &shader);
    scene.genAnts(1000);
    scene.genSources(2);

    float lastTime = 0.0f;
    float alpha = 1.0f; 
    while (!glfwWindowShouldClose(window.glWindow_))
    {
        double time = glfwGetTime();
        double deltaTime = time - lastTime;

        if( deltaTime >= MAX_PERIOD ) {
            lastTime = time;
            alpha += 0.001f;
            scene.update(alpha);
            scene.render(); 
        }
    }

    glfwTerminate();
    return 0;
}