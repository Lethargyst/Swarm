#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Renderer
{
    class ShaderProgram
    {
    public:
        ShaderProgram(){};
        ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

        GLuint const getID() const { return ID_; }  
        bool isCompiled() const { return isCompiled_; }
        void use() const { glUseProgram(ID_); }

    private:
        bool compileShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
        GLuint ID_;
        bool isCompiled_ = false;
    };

    GLuint createVBO(float* vertexes, GLuint size, GLenum usage);
    GLuint createVAO();
    bool loadSource(const std::string& filePath, std::string& source);
    void initGLFW(const int major, const int minor);
}

#endif