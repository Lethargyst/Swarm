#include "ShaderProgram.h"

namespace Renderer
{
    ShaderProgram::ShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
    {
        GLuint vertexShader, fragmentShader;
        if (!compileShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShader)) return;
        if (!compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader)) return;

        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);

        int success;
        char infoLog[512];
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cerr << "SHADER PROGRAM: LINKING ERROR\n" << infoLog << std::endl;
            return;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        isCompiledFlag = true;
    }

    bool ShaderProgram::compileShader(const std::string &source, const GLenum shaderType, GLuint &shaderID)
    {
        const char *c_source = source.c_str();
        
        shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &c_source, nullptr);
        glCompileShader(shaderID);

        int success;
        char infoLog[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
            std::cerr << shaderType << ": COMPILATION ERROR\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }
}
