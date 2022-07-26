#include <iostream>
#include <math.h>
#include <chrono>
#include "Math/Vectors.h"
#include "Render/ShaderProgram.h"
#include "Render/Window.h"
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

    std::string vertexShaderSource, fragmentShaderSource;
    Renderer::loadSource("../src/Render/Shaders/VertexShader.vert", vertexShaderSource);
    Renderer::loadSource("../src/Render/Shaders/FragmentShader.frag", fragmentShaderSource);
    Renderer::ShaderProgram shader(vertexShaderSource, fragmentShaderSource);


    float vertexes[] = {
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    Scene& scene = Scene::initialize();

    scene.initBuffers(1);
    scene.setBufferData(0, sizeof(vertexes), vertexes, GL_DYNAMIC_DRAW);
    scene.configBuffer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    float alpha = 0.0f;
    while (!glfwWindowShouldClose(window.glWindow_))
    {
        auto start_time = std::chrono::steady_clock::now();

        window.processInput();

        scene.update(alpha);
        shader.use();
        scene.render();
    
        glfwSwapBuffers(window.glWindow_);
        glfwPollEvents();

        alpha += 0.001f;

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_ns = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        printf("FPS: %f\n", 1.0f / (elapsed_ns.count() / 1e6));
    }

    glfwTerminate();
    return 0;
}
