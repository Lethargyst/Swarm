#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <vector>
#include "../Objects/Objects.h"
#include "../Render/ShaderProgram.h"

class Scene
{
public:
    static Scene& initialize();

    void initBuffers(GLsizei num);
    void setBufferData(GLint bufferIndex, GLsizei size, void* data, GLenum usage);
    void configBuffer(GLuint index, GLsizei size, GLenum type, 
                      GLboolean normalize, GLsizei stride, const void* offset);
    void updateBuffer(GLint bufferIndex, GLsizei size, void* data, GLenum usage);
    void update(const float alpha);
    void render() const;

private:
    Scene() {}
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete; 

    static std::vector<Object*> objects;
    static float* renderInfo;
    GLuint *VAO_, *VBO_; 
    GLsizei buffersAmount_;
};


#endif