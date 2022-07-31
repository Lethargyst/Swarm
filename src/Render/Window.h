#ifndef WINDOW_H
#define WINFOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "../Math/Vectors.h"


class Window
{
public:
    Window(int width, int height, const char* label);
    ~Window() { if (glWindow_) delete glWindow_; }

    void processInput();
    vec2 getResolution() const { return vec2(width_, height_); }

    GLFWwindow *glWindow_;
private:
    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

    static int width_, height_;
};

#endif