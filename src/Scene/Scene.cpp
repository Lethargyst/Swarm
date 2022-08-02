#include "Scene.h"

float* Scene::renderInfo;

Scene& Scene::initialize(Window* window, Renderer::ShaderProgram* shader)
{
    static Scene sceneObj(window, shader);
    return sceneObj;
}

Scene::~Scene() 
{ 
    if (renderInfo) delete[] renderInfo; 
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
    renderInfo = (float*)data;
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
    renderInfo = (float*)data;
}

void Scene::update(const float alpha)
{
    window_->processInput();

    for (std::size_t i = 0; i < Ant::getAmount(); ++i)
        ants[i]->update(alpha);
    for (std::size_t i = 0; i < Source::getAmount(); ++i) 
        sources[i]->update(alpha);
}

void Scene::render() const
{
    shader_->use();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vec2 resolution = window_->getResolution();
    glUniform2f(glGetUniformLocation(shader_->getID(), "resolution"), resolution.x, resolution.y);
    //---------------------------------------

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), renderInfo);

    for (GLsizei i = 0; i < buffersAmount_; ++i) {
        glBindVertexArray(VAO_[i]);
        glDrawArrays(GL_POINTS, 0, 3);
    }

    //---------------------------------------
    glfwSwapBuffers(window_->glWindow_);
    glfwPollEvents();
}