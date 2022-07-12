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

        bool isCompiled() const { return isCompiled_; }
        void use() const { glUseProgram(ID_); }

    private:
        bool compileShader(const std::string &source, const GLenum shaderType, GLuint &shaderID);

        GLuint ID_;
        bool isCompiled_ = false;
    };

    GLuint createVBO(float *vertexes, GLuint size);
    GLuint createVAO(float *vertexes, GLuint size);
    bool loadSource(const char *fileName, std::string &source);
}

#endif