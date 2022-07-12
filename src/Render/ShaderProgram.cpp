#include "ShaderProgram.h"

namespace Renderer
{
    ShaderProgram::ShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
    {
        GLuint vertexShader, fragmentShader;
        if (!compileShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShader)) return;
        if (!compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader)) return;

        glAttachShader(ID_, vertexShader);
        glAttachShader(ID_, fragmentShader);
        glLinkProgram(ID_);

        int success;
        char infoLog[512];
        glGetProgramiv(ID_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID_, 512, NULL, infoLog);
            std::cerr << "Shader Program: LINKING ERROR\n" << infoLog << std::endl;
            return;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        isCompiled_ = true;
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

    GLuint createVBO(float *vertexes, GLuint size)
    {
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, vertexes, GL_DYNAMIC_DRAW);
        return VBO;
    }

    GLuint createVAO(float *vertexes, GLuint size)
    {
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLuint VBO = createVBO(vertexes, size);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        return VAO;
    }

    bool loadSource(const char *fileName, std::string &source)
    {
        source = "";
        FILE* file = fopen(fileName, "r");
        if (!file) {
            std::cerr << "Shader Program: CAN'T OPEN FILE" << std::endl;
            return false;
        }

        fseek(file, 0, SEEK_END);
        long int size = ftell(file);
        fseek(file, 0, SEEK_SET);
        void *buffer = malloc(size + 1);
        fread(buffer, 1, size, file);
        fclose(file);
        ((char*)buffer)[size] = '\0';

        source = (char*)buffer;
        return true;
    }
}
