#include <iostream>
#include <math.h>
#include "Render/ShaderProgram.h"
#include "Render/Window.h"
#include "Math/Vectors.h"


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
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top 
    };
    // float vertexes[] = {
    //     0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f
    // };
    GLuint VBO = Renderer::createVBO(vertexes, sizeof(vertexes), GL_STATIC_DRAW);
    GLuint VAO = Renderer::createVAO();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window.glWindow_))
    {
        window.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
        
    
        glfwSwapBuffers(window.glWindow_);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
