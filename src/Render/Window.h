#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "../Math/Vectors.h"
#include "../Objects/Geometry2D.h"


class Window
{
public:
    Window(int width, int height, const char* label);
    ~Window() {}

    vec2 getResolution() const { return vec2(width, height); }
    Rectangle2d& getBounds() const;

    GLFWwindow *glWindow_;
private:
    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    static void frameBufferSizeCallback(GLFWwindow* window, int window_width, int window_height);

    static Rectangle2d bounds;    
    static int width, height;
};

#endif