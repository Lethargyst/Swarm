#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include <glad/glad.h>
#include <string>
#include <iostream>

namespace Renderer      
{
    class ShaderProgram
    {
    public:
        ShaderProgram(){};
        ShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

        bool isCompiled() const { return isCompiledFlag; }

    private:
        bool compileShader(const std::string &source, const GLenum shaderType, GLuint &shaderID);

        GLuint ID;
        bool isCompiledFlag = false;
    };
}

#endif