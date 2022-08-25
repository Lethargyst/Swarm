#include "Scene.h"

float* Scene::objectsRenderBuffer = new float[RENDER_BUFFER_SIZE];
float* Scene::quadTreeRenderBuffer = new float[QUAD_TREE_BUFFER_SIZE];

Scene::~Scene() 
{ 
    if (objectsRenderBuffer) delete[] objectsRenderBuffer; 
    if (VAO_) delete[] VAO_;
    if (VBO_) delete[] VBO_;
    if (window_) delete window_;
    if (quadTree_) delete quadTree_;
}

Scene& Scene::initialize(Window* window)
{
    static Scene sceneObj(window);

    sceneObj.loadShaders();
    sceneObj.initBuffers(2);

    // objects are represented in objects render buffer as follows:
    // [..., pos.x, pos.y, size, color.x, color.y, color.z, ...] 
    sceneObj.setBufferData(0, RENDER_BUFFER_SIZE, objectsRenderBuffer, GL_DYNAMIC_DRAW);
    sceneObj.configBuffer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr); // object coordinates
    sceneObj.configBuffer(1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float))); // object size
    sceneObj.configBuffer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // object color

    // quad tree render buffer: [..., pos.x, pos.y, ...]
    sceneObj.setBufferData(1, QUAD_TREE_BUFFER_SIZE, quadTreeRenderBuffer, GL_DYNAMIC_DRAW);
    sceneObj.configBuffer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    vec2 resolution = window->getResolution();
    sceneObj.quadTree_ = new QuadTree(Rectangle2d(0.0f, 0.0f, resolution.x, resolution.y));

    return sceneObj;
}

void Scene::loadShaders()
{
    using namespace Renderer;

    // Objects shader
    std::string objectsVertexShaderSource, objectsFragmentShaderSource, objectsGeometryShaderSource;
    loadSource("../src/Render/Shaders/Objects/VertexShader.vert", objectsVertexShaderSource);
    loadSource("../src/Render/Shaders/Objects/FragmentShader.frag", objectsFragmentShaderSource);
    loadSource("../src/Render/Shaders/Objects/GeometryShader.geom", objectsGeometryShaderSource);

    ShaderProgram *objectsShader = new ShaderProgram;
    objectsShader->compileShader(objectsVertexShaderSource, GL_VERTEX_SHADER);
    objectsShader->compileShader(objectsFragmentShaderSource, GL_FRAGMENT_SHADER);
    objectsShader->compileShader(objectsGeometryShaderSource, GL_GEOMETRY_SHADER);
    objectsShader->linkShaders();
    shaders_.push_back(objectsShader);

    // Quad tree shader
    std::string quadTreeVertexShaderSource, quadTreeFragmentShaderSource;
    loadSource("../src/Render/Shaders/QuadTree/VertexShader.vert", quadTreeVertexShaderSource);
    loadSource("../src/Render/Shaders/Objects/FragmentShader.frag", quadTreeFragmentShaderSource);

    ShaderProgram *quadTreeShader = new ShaderProgram;
    quadTreeShader->compileShader(quadTreeVertexShaderSource, GL_VERTEX_SHADER);
    quadTreeShader->compileShader(quadTreeFragmentShaderSource, GL_FRAGMENT_SHADER);
    quadTreeShader->linkShaders();
    shaders_.push_back(quadTreeShader);
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
}

void Scene::configBuffer(GLuint index, GLsizei size, GLenum type, 
                         GLboolean normalize, GLsizei stride, const void* offset)
{
    glVertexAttribPointer(index, size, type, normalize, stride, offset);	
    glEnableVertexAttribArray(index);
}

void Scene::genAnts(GLint num)
{
    vec2 resolution = window_->getResolution();
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(rand() % (int)resolution.x, rand() % (int)resolution.y);
        vec3 color = vec3(255.0f, 255.0f, 255.0f);
        Ant* ant = new Ant(pos, 25.0f, 2.0f, ANT_SIZE, color);

        Rectangle2d bounds = Rectangle2d(ant->pos_ - ant->size_, ant->size_);
        QuadTreeData* data = new QuadTreeData(ant, bounds);        

        quadTree_->insert(*data);
        quadTreeDataSet_.push_back(data);
        ants.push_back(ant);
    }
}

void Scene::genSources(GLint num)
{
    vec2 resolution = window_->getResolution();
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(rand() % (int)resolution.x, rand() % (int)resolution.y);
        vec3 color = vec3((int)pos.x % 255, (int)pos.y % 255, (int)rand() % 255);
        sources.push_back(new Source(pos, 2.0f, SOURCE_SIZE, color));
    }
}

void Scene::updateObjectRenderInfo(GLint i, Object* obj)
{
    objectsRenderBuffer[i * 6] = obj->pos_.x;          
    objectsRenderBuffer[i * 6 + 1] = obj->pos_.y;      
    objectsRenderBuffer[i * 6 + 2] = obj->size_;      
    objectsRenderBuffer[i * 6 + 3] = obj->color_.x;   
    objectsRenderBuffer[i * 6 + 4] = obj->color_.y;   
    objectsRenderBuffer[i * 6 + 5] = obj->color_.z;   
}

void Scene::update(const float alpha)
{
    processInput();

    // Updating ants objects
    for (std::size_t i = 0, size = Ant::getAmount(); i < size; ++i) {
         ants[i]->update(alpha);
        updateObjectRenderInfo(i, ants[i]);
    }

    // Updating sources objects    
    for (std::size_t i = 0, size = Source::getAmount(); i < size; ++i) {
        sources[i]->update(alpha);
        updateObjectRenderInfo(i + Ant::getAmount(), sources[i]);
    }

    // Updating quad tree
    quadTree_->clear();
    for (std::size_t i = 0, size = quadTreeDataSet_.size(); i < size; ++i) {
        quadTree_->insert(*quadTreeDataSet_[i]);
    }

    // Updating quad tree rendering data
    // if (renderingQuadTree) {
    //
    // }

    objectsAmount_ = Ant::getAmount() + Source::getAmount();
}

void Scene::render() const
{
    shaders_[0]->use();

    // Clearing the monitor
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Sending resolution value to the graphics shader
    vec2 resolution = window_->getResolution();
    glUniform2f(glGetUniformLocation(shaders_[0]->getID(), "resolution"), resolution.x, resolution.y);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    // Sending renering info from objectsRenderBuffer to the graphics shader
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, objectsAmount_ * 6 * sizeof(float), objectsRenderBuffer);

    glBindVertexArray(VAO_[0]);
    glDrawArrays(GL_POINTS, 0, objectsAmount_);

    // if (renderingQuadTree) {
    //     glBindBuffer(GL_ARRAY_BUFFER, VBO_[1]);
    //     glBufferSubData(GL_ARRAY_BUFFER, 0, ..., quadTreeRenderBuffer);

    //     glBindVertexArray(VAO_[1]);
    //     glDrawArrays(GL_LINES, 0, ...);
    // }

    glfwSwapBuffers(window_->glWindow_);
    glfwPollEvents();
}

void Scene::processInput()
{
    if (glfwGetKey(window_->glWindow_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_->glWindow_, true); 

    if (glfwGetKey(window_->glWindow_, GLFW_KEY_SPACE) == GLFW_PRESS)
        renderingQuadTree = !renderingQuadTree;
}