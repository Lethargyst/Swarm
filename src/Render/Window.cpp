#include "window.h"

int Window::width_ = 0;
int Window::height_ = 0;

Window::Window(int width, int height, const char* label)
{
    width_ = width;
    height_ = height;
    glWindow_ = glfwCreateWindow(width_, height_, label, nullptr, nullptr);
    if (!glWindow_)
    {
        std::cerr << "Window: CAN'T CREATE WINDOW" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(glWindow_);
    glfwSetFramebufferSizeCallback(glWindow_, frameBufferSizeCallback);
}

void Window::processInput()
{
    if (glfwGetKey(glWindow_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glWindow_, true);    
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    width_ = width;
    height_ = height;
}