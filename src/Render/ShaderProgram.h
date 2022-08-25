#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace Renderer
{
    class ShaderProgram
    {
    public:
        ShaderProgram() {}

        GLuint const getID() const { return ID_; }  
        bool compileShader(const std::string& shaderSource, GLenum shaderType);
        bool linkShaders();
        bool isCompiled() const { return isCompiled_; }
        void use() const { glUseProgram(ID_); }

    private:
        std::vector<GLuint> shaders_;
    
        GLuint ID_;
        bool isCompiled_ = false;
    };

    bool loadSource(const std::string& filePath, std::string& source);
    void initGLFW(const int major, const int minor);
}

#endif