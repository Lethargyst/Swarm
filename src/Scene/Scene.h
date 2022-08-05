#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <vector>
#include "../Objects/Swarm.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Window.h"
#include "../precompiled.h"

class Scene
{
public:
    static Scene& initialize(Window* window, Renderer::ShaderProgram* shader);

    void initBuffers(GLsizei num);
    void setBufferData(GLint bufferIndex, GLsizei size, void* data, GLenum usage);
    void configBuffer(GLuint index, GLsizei size, GLenum type, 
                      GLboolean normalize, GLsizei stride, const void* offset);
    void updateBuffer(GLint bufferIndex, GLsizei size, void* data, GLenum usage);
    void update(const float alpha);
    void updateObjectRenderInfo(GLint i, Object* obj);
    void render() const;

    GLint objectsAmount_;
private:
    Scene() {}
    Scene(Window* window, Renderer::ShaderProgram* shader) : window_(window), shader_(shader) {} 
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete; 

    static float* renderBuffer;

    std::vector<Ant*> ants;
    std::vector<Source*> sources;
    Window* window_;
    Renderer::ShaderProgram* shader_;
    GLuint *VAO_, *VBO_; 
    GLsizei buffersAmount_;
};


#endif