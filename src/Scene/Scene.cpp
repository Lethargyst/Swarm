#include "Scene.h"

std::vector<Object*> Scene::objects;
float* Scene::renderInfo;

Scene& Scene::initialize(Window* window, GLuint shader)
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
    for (size_t i = 0; i < 3; ++i) {
        renderInfo[2 * i] = cosf((2.0f / 3.0f * M_PI) * i + alpha) / 2;
        renderInfo[2 * i + 1] = sinf((2.0f / 3.0f * M_PI) * i + alpha) / 2;
    }   
}

void Scene::render() const
{
    vec2 resolution = window_->getResolution();
    glUniform2f(glGetUniformLocation(shader_, "resolution"), resolution.x, resolution.y);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), renderInfo);

    for (GLsizei i = 0; i < buffersAmount_; ++i) {
        glBindVertexArray(VAO_[i]);
        glDrawArrays(GL_POINTS, 0, 3);
    }
}