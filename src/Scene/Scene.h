#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <vector>
#include "../precompiled.h"
#include "../Scene/Swarm.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Window.h"
#include "QuadTree.h"

class Scene
{
public:
    static Scene& initialize(Window* window);

    // void genAnts(GLint num);
    // void genSources(GLint num);
    void update(const float alpha);

    // Updates information about the object in the objectsRenderBuffer
    // i - index of the object in the objectsRenderBuffer,
    // obj - pointer to the object
    void initSwarm(int antsNum, int sourcesNum);
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

    void updateObjectRenderInfo(GLint i, Object* obj);
    void updateQuadTreeBuffer();
    void updateSwarmRenderInfo();
    void loadShaders();

    std::vector<Renderer::ShaderProgram*> shaders_;

    QuadTree quadTree_;
    Swarm swarm;

    static float* objectsRenderBuffer;
    static float* quadTreeRenderBuffer;
    
    Window* window_;
    GLuint *VAO_, *VBO_; 

    bool renderingQuadTree = false;
};


#endif