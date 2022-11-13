#include <iostream>
#include <math.h>
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

    Scene& scene = Scene::initialize(&window);
    scene.initBuffers();
    
    int antsNum = 1000;
    sscanf(argv[1], "%i", &antsNum);
    scene.initSwarm(antsNum, 2);

    float lastTime = 0.0f;
    float alpha = 1.0f; 
    while (!glfwWindowShouldClose(window.glWindow_))
    {
        double time = glfwGetTime();
        double deltaTime = time - lastTime;

        if (deltaTime >= MAX_PERIOD) {
            lastTime = time;
            alpha += 0.001f;
            scene.update(1.0f);
            scene.render();
        }
    }

    glfwTerminate();

    return 0;
}