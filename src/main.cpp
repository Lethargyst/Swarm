#include "Scene/Scene.hpp"
#include "Config/Config.hpp"
#include <sstream>
#include <thread>

void showFPS(Window& window, double deltaTime)
{
    double fps = 1 / deltaTime;
    std::stringstream ss;
    ss << "Swarm " << "[" << fps << " FPS]";
    glfwSetWindowTitle(window.glWindow_, ss.str().c_str());
}

int main(int argc, char* argv[])
{
    Renderer::initGLFW(4, 6);
    Window window(800, 600, "Swarm");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Scene& scene = Scene::initialize(&window);
    
    scene.initSwarm(global.ants_amount, global.sources_amount);

    float lastTime = 0.0f;
    float alpha = 1.0f; 
    while (!glfwWindowShouldClose(window.glWindow_))
    {
        double time = glfwGetTime();
        double deltaTime = time - lastTime;

        if (deltaTime >= global.max_period) {
            lastTime = time;
            alpha += 0.001f;
            
            scene.update(1.0f);
            scene.render();
            showFPS(window, deltaTime);
        }
    }

    glfwTerminate();

    return 0;
}

