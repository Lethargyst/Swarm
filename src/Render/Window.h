#ifndef WINDOW_H
#define WINFOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>


class Window
{
public:
    Window(int width, int height, const char* label);
    ~Window() { if (glWindow_) delete glWindow_; }

    void processInput();

    GLFWwindow *glWindow_;
private:
    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

    static int width_, height_;
};

#endif