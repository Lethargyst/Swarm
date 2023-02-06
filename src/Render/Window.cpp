#include "Window.hpp"

Rectangle2d Window::bounds;
int Window::width = 0;
int Window::height = 0;

Window::Window(int width, int height, const char* label)
{
    this->width = width;
    this->height = height;
    glWindow_ = glfwCreateWindow(this->width, this->height, label, nullptr, nullptr);
    if (!glWindow_)
    {
        std::cerr << "Window: CAN'T CREATE WINDOW" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(glWindow_);
    glfwSetFramebufferSizeCallback(glWindow_, frameBufferSizeCallback);

    bounds = Rectangle2d(0.0f, 0.0f, (float)width, (float)height);
}

Rectangle2d& Window::getBounds() const
{
    return bounds;
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int window_width, int window_height)
{
    glViewport(0, 0, width, height);
    width = window_width;
    height = window_height;
    bounds.size_.x = window_width;
    bounds.size_.y = window_height;
}   
