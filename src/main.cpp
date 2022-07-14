#include <iostream>
#include "Render/ShaderProgram.h"
#include "Render/Window.h"


int main(int argc, char* argv[])
{
    Renderer::initGLFW(3, 3);
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

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    GLuint VAO = Renderer::createVAO(vertices, sizeof(vertices));

    while (!glfwWindowShouldClose(window.glWindow_))
    {
        window.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    
        glfwSwapBuffers(window.glWindow_);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
