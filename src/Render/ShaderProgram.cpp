#include "ShaderProgram.h"

namespace Renderer
{
    ShaderProgram::ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
    {
        GLuint vertexShader, fragmentShader;
        if (!compileShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShader))
            return;
        if (!compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader))
            return;

        ID_ = glCreateProgram();
        glAttachShader(ID_, vertexShader);
        glAttachShader(ID_, fragmentShader);
        glLinkProgram(ID_);

        int success;
        glGetProgramiv(ID_, GL_LINK_STATUS, &success);
        if (!success) {
            std::cerr << "Shader Program: LINKING ERROR\n" << std::endl;
            return;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        isCompiled_ = true;
    }

    bool ShaderProgram::compileShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
    {
        const char* c_source = source.c_str();

        shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &c_source, nullptr);
        glCompileShader(shaderID);

        int success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            std::cerr << "Shader Program: COMPILATION ERROR\n" << std::endl;
            return false;
        }
        return true;
    }

    bool loadSource(const std::string& filePath, std::string& source)
    {
        std::ifstream file; 
        file.open(filePath);
        if (!file) {
            std::cerr << "Shader Program: CAN'T OPEN FILE" << std::endl;
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        source = buffer.str();
        file.close();
        return true;
    }

    void initGLFW(const int major, const int minor)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    }
}
