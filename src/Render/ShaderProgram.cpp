#include "ShaderProgram.h"

namespace Renderer
{
    // ShaderProgram::ShaderProgram(const std::string& vertexShaderSource, 
    //                              const std::string& fragmentShaderSource,
    //                              const std::string& geometryShaderSource)
    // {
    //     GLuint vertexShader, fragmentShader, geometryShader;
    //     if (!compileShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShader))
    //         return;
    //     if (!compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader))
    //         return;
    //     if (!compileShader(geometryShaderSource, GL_GEOMETRY_SHADER, geometryShader))
    //         return;

    //     ID_ = glCreateProgram();
    //     glAttachShader(ID_, vertexShader);
    //     glAttachShader(ID_, fragmentShader);
    //     glAttachShader(ID_, geometryShader);
    //     glLinkProgram(ID_);

    //     int success;
    //     char infoLog[512];
    //     glGetProgramiv(ID_, GL_LINK_STATUS, &success);
    //     if (!success) {
    //         glGetProgramInfoLog(ID_, 512, NULL, infoLog);
    //         std::cerr << "Shader Program: LINKING ERROR\n" 
    //                     << "----------------------------------\n" 
    //                     << infoLog << "\n"
    //                     << "----------------------------------\n";
    //         return;
    //     }

    //     glDeleteShader(vertexShader);
    //     glDeleteShader(fragmentShader);
    //     glDeleteShader(geometryShader);
    //     isCompiled_ = true;

    //     // printf("Shaders initialized succesfully\n");
    // }

    // bool ShaderProgram::compileShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
    // {
    //     const char* c_source = source.c_str();

    //     shaderID = glCreateShader(shaderType);
    //     glShaderSource(shaderID, 1, &c_source, nullptr);
    //     glCompileShader(shaderID);

    //     int success;
    //     char infoLog[512];
    //     glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    //     if (!success) {
    //         glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    //         std::cerr << "Shader Program: COMPILATION ERROR\n" 
    //                     << "----------------------------------\n" 
    //                     << infoLog  << "\n"
    //                     << "----------------------------------\n";
    //         return false;
    //     }
    //     return true;
    // }

    bool ShaderProgram::compileShader(const std::string& source, GLenum shaderType)
    {
        const char* c_source = source.c_str();

        GLuint shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &c_source, nullptr);
        glCompileShader(shaderID);

        int success;
        char infoLog[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
            std::cerr << "Shader Program: COMPILATION ERROR\n" 
                        << "----------------------------------\n"
                        << infoLog  << "\n"
                        << "----------------------------------\n";
            return false;
        }

        shaders_.push_back(shaderID);
        return true;
    }

    bool ShaderProgram::linkShaders()
    {
        ID_ = glCreateProgram();
        for (std::size_t i = 0, size = shaders_.size(); i < size; ++i) {
            glAttachShader(ID_, shaders_[i]);
            glDeleteShader(shaders_[i]);
        }
        glLinkProgram(ID_);

        int success;
        char infoLog[512];
        glGetProgramiv(ID_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID_, 512, NULL, infoLog);
            std::cerr << "Shader Program: LINKING ERROR\n" 
                        << "----------------------------------\n" 
                        << infoLog << "\n"
                        << "----------------------------------\n";
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
