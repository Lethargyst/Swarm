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

    void genAnts(GLint num);
    void genSources(GLint num);
    void update(const float alpha);

    // Updates information about the object in the objectsRenderBuffer
    // i - index of the object in the objectsRenderBuffer,
    // obj - pointer to the object
    void updateObjectRenderInfo(GLint i, Object* obj);
    void processInput();    
    void initBuffers();
    void render() const;

    GLint objectsAmount_;
private:
    Scene() {}
    Scene(Window* window) : window_(window) {} 
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete; 

    void loadShaders();
    void updateQuadTreeBuffer();

    QuadTree quadTree_;

    std::vector<Object*> ants;  
    std::vector<Object*> sources;
    std::vector<Renderer::ShaderProgram*> shaders_;

    static float* objectsRenderBuffer;
    static float* quadTreeRenderBuffer;
    
    Window* window_;
    GLuint *VAO_, *VBO_; 

    bool renderingQuadTree = false;
};


#endif