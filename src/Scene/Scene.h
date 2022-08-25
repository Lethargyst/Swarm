#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <vector>
#include "../precompiled.h"
#include "../Objects/Swarm.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Window.h"
#include "QuadTree.h"

class Scene
{
public:
    static Scene& initialize(Window* window);

    void initBuffers(GLsizei num);
    void setBufferData(GLint bufferIndex, GLsizei size, void* data, GLenum usage);
    void configBuffer(GLuint index, GLsizei size, GLenum type, 
                      GLboolean normalize, GLsizei stride, const void* offset);

    void genAnts(GLint num);
    void genSources(GLint num);

    void update(const float alpha);

    // Updates information about the object in the objectsRenderBuffer
    // i - index of the object in the objectsRenderBuffer,
    // obj - pointer to the object
    void updateObjectRenderInfo(GLint i, Object* obj);
    void processInput();
    
    void render() const;

    GLint objectsAmount_;
private:
    Scene() {}
    Scene(Window* window) : window_(window) {} 
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete; 

    void loadShaders();

    static float* objectsRenderBuffer;
    static float* quadTreeRenderBuffer;

    std::vector<Ant*> ants;
    std::vector<Source*> sources;
    std::vector<QuadTreeData*> quadTreeDataSet_;
    std::vector<Renderer::ShaderProgram*> shaders_;

    QuadTree* quadTree_;
    Window* window_;
    GLuint *VAO_, *VBO_; 
    GLsizei buffersAmount_;

    bool renderingQuadTree = false;
};


#endif