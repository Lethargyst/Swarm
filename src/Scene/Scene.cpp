#include "Scene.h"

float* Scene::renderBuffer = new float[RENDER_BUFFER_SIZE];

Scene& Scene::initialize(Window* window, Renderer::ShaderProgram* shader)
{
    static Scene sceneObj(window, shader);
    sceneObj.initBuffers(1);
    sceneObj.setBufferData(0, sizeof(renderBuffer), renderBuffer, GL_DYNAMIC_DRAW);

    // objects are represented in render buffer as follows:
    // [..., pos.x, pos.y, size, color.x, color.y, color.z, ...] 
    
    sceneObj.configBuffer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr); // object coordinates
    sceneObj.configBuffer(1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float))); // object size
    sceneObj.configBuffer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // object color

    return sceneObj;
}

Scene::~Scene() 
{ 
    if (renderBuffer) delete[] renderBuffer; 
    if (VAO_) delete[] VAO_;
    if (VBO_) delete VBO_;
    if (window_) delete window_;
}

void Scene::initBuffers(GLsizei num)
{
    VAO_ = new GLuint[num];
    VBO_ = new GLuint[num];
    glGenVertexArrays(num, VAO_); 
    glGenBuffers(num, VBO_);
    buffersAmount_ = num;
}

void Scene::setBufferData(GLint bufferIndex, GLint size, void* data, GLenum usage)
{
    glBindVertexArray(VAO_[bufferIndex]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[bufferIndex]);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    renderBuffer = (float*)data;
}

void Scene::configBuffer(GLuint index, GLsizei size, GLenum type, 
                         GLboolean normalize, GLsizei stride, const void* offset)
{
    glVertexAttribPointer(index, size, type, normalize, stride, offset);	
    glEnableVertexAttribArray(index);
}

void Scene::updateBuffer(GLint bufferIndex, GLsizei size, void* data, GLenum usage)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[bufferIndex]);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage); 
    renderBuffer = (float*)data;
}

void Scene::update(const float alpha)
{
    window_->processInput();

    for (std::size_t i = 0; i < Ant::getAmount(); ++i) {
        ants[i]->update(alpha);
        updateObjectRenderInfo(i * 6, ants[i]);
    }
        
    for (std::size_t i = 0; i < Source::getAmount(); ++i) {
        sources[i]->update(alpha);
        updateObjectRenderInfo(i * 6, ants[i]);
    }

    objectsAmount_ = Ant::getAmount() + Source::getAmount();
}

void Scene::updateObjectRenderInfo(GLint i, Object* obj)
{
    renderBuffer[i] = obj->pos_.x;
    renderBuffer[i + 1] = obj->pos_.y;
    renderBuffer[i + 2] = obj->size_;
    renderBuffer[i + 3] = obj->color_.x;
    renderBuffer[i + 4] = obj->color_.y;
    renderBuffer[i + 5] = obj->color_.z;
}

void Scene::render() const
{
    shader_->use();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vec2 resolution = window_->getResolution();
    glUniform2f(glGetUniformLocation(shader_->getID(), "resolution"), resolution.x, resolution.y);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, objectsAmount_ * 6 * sizeof(float), renderBuffer);

    for (GLsizei i = 0; i < buffersAmount_; ++i) {
        glBindVertexArray(VAO_[i]);
        glDrawArrays(GL_POINTS, 0, objectsAmount_);
    }

    glfwSwapBuffers(window_->glWindow_);
    glfwPollEvents();
}