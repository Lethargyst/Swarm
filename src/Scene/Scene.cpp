#include "Scene.h"

float* Scene::renderBuffer = new float[RENDER_BUFFER_SIZE];

Scene& Scene::initialize(Window* window, Renderer::ShaderProgram* shader)
{
    static Scene sceneObj(window, shader);
    sceneObj.initBuffers(1);
    sceneObj.setBufferData(0, RENDER_BUFFER_SIZE, renderBuffer, GL_DYNAMIC_DRAW);

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
    if (VBO_) delete[] VBO_;
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

void Scene::genAnts(GLint num)
{
    vec2 resolution = window_->getResolution();
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(rand() % (int)resolution.x, rand() % (int)resolution.y);
        vec3 color = vec3(255.0f, 255.0f, 255.0f);
        ants.push_back(new Ant(pos, 25.0f, 10.0f, ANT_SIZE, color));
    }
}

void Scene::genSources(GLint num)
{
    vec2 resolution = window_->getResolution();
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(rand() % (int)resolution.x, rand() % (int)resolution.y);
        vec3 color = vec3((int)pos.x % 255, (int)pos.y % 255, (int)rand() % 255);
        sources.push_back(new Source(pos, 5.0f, SOURCE_SIZE, color));
    }
}

void Scene::updateObjectRenderInfo(GLint i, Object* obj)
{
    renderBuffer[i * 6] = obj->pos_.x;          
    renderBuffer[i * 6 + 1] = obj->pos_.y;      
    renderBuffer[i * 6 + 2] = obj->size_;      
    renderBuffer[i * 6 + 3] = obj->color_.x;   
    renderBuffer[i * 6 + 4] = obj->color_.y;   
    renderBuffer[i * 6 + 5] = obj->color_.z;   
}

void Scene::update(const float alpha)
{
    window_->processInput();

    // Updating ants objects
    for (std::size_t i = 0; i < Ant::getAmount(); ++i) {
        ants[i]->update(alpha);
        updateObjectRenderInfo(i, ants[i]);
    }

    // Updating sources objects    
    for (std::size_t i = 0; i < Source::getAmount(); ++i) {
        sources[i]->update(alpha);
        updateObjectRenderInfo(i + Ant::getAmount(), sources[i]);
    }

    objectsAmount_ = Ant::getAmount() + Source::getAmount();
}

void Scene::render() const
{
    shader_->use();

    // Clearing the monitor
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Sending resolution value to the graphics shader
    vec2 resolution = window_->getResolution();
    glUniform2f(glGetUniformLocation(shader_->getID(), "resolution"), resolution.x, resolution.y);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    // Sending renering info from renderBuffer to the graphics shader
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);

    glBufferSubData(GL_ARRAY_BUFFER, 0, objectsAmount_ * 6 * sizeof(float), renderBuffer);

    glBindVertexArray(VAO_[0]);
    glDrawArrays(GL_POINTS, 0, objectsAmount_);

    glfwSwapBuffers(window_->glWindow_);
    glfwPollEvents();
}