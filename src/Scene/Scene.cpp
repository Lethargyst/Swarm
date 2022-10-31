#include "Scene.h"

float* Scene::objectsRenderBuffer = new float[RENDER_BUFFER_SIZE];
float* Scene::quadTreeRenderBuffer = new float[QUAD_TREE_BUFFER_SIZE];

Scene::~Scene() 
{ 
    if (objectsRenderBuffer) delete[] objectsRenderBuffer; 
    if (VAO_) delete[] VAO_;
    if (VBO_) delete[] VBO_;
    if (window_) delete window_;
}

Scene& Scene::initialize(Window* window)
{
    static Scene sceneObj(window);

    vec2 resolution = window->getResolution();

    return sceneObj;
}

void Scene::initBuffers()
{
    loadShaders();
    VAO_ = new GLuint[2];
    VBO_ = new GLuint[2];
    glGenVertexArrays(2, VAO_); 
    glGenBuffers(2, VBO_);

    // objects are represented in objects render buffer as follows:
    // [..., pos.x, pos.y, size, color.x, color.y, color.z, ...] 
    glBindVertexArray(VAO_[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
    glBufferData(GL_ARRAY_BUFFER, RENDER_BUFFER_SIZE, objectsRenderBuffer, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr); // object coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float))); // object size
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // object color
    glEnableVertexAttribArray(2);
    // quad tree render buffer: [..., pos.x, pos.y, size.x, size.y, ...]
    glBindVertexArray(VAO_[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[1]);
    glBufferData(GL_ARRAY_BUFFER, QUAD_TREE_BUFFER_SIZE, quadTreeRenderBuffer, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Scene::loadShaders()
{
    using namespace Renderer;

    // Objects shader
    std::string objectsVertexShaderSource, objectsFragmentShaderSource, objectsGeometryShaderSource;
    loadSource("../src/Render/Shaders/Objects/VertexShader.vert", objectsVertexShaderSource);
    loadSource("../src/Render/Shaders/Objects/FragmentShader.frag", objectsFragmentShaderSource);
    loadSource("../src/Render/Shaders/Objects/GeometryShader.geom", objectsGeometryShaderSource);

    ShaderProgram *objectShader = new ShaderProgram;
    objectShader->compileShader(objectsVertexShaderSource, GL_VERTEX_SHADER);
    objectShader->compileShader(objectsFragmentShaderSource, GL_FRAGMENT_SHADER);
    objectShader->compileShader(objectsGeometryShaderSource, GL_GEOMETRY_SHADER);
    objectShader->linkShaders();
    shaders_.push_back(objectShader);

    // Quad tree shader
    std::string quadTreeVertexShaderSource, quadTreeFragmentShaderSource, quadTreeGeometryShaderSource;
    loadSource("../src/Render/Shaders/QuadTree/VertexShader.vert", quadTreeVertexShaderSource);
    loadSource("../src/Render/Shaders/QuadTree/FragmentShader.frag", quadTreeFragmentShaderSource);
    loadSource("../src/Render/Shaders/QuadTree/GeometryShader.geom", quadTreeGeometryShaderSource);

    ShaderProgram *quadTreeShader = new ShaderProgram;
    quadTreeShader->compileShader(quadTreeVertexShaderSource, GL_VERTEX_SHADER);
    quadTreeShader->compileShader(quadTreeFragmentShaderSource, GL_FRAGMENT_SHADER);
    quadTreeShader->compileShader(quadTreeGeometryShaderSource, GL_GEOMETRY_SHADER);
    quadTreeShader->linkShaders();
    shaders_.push_back(quadTreeShader);
}

void Scene::initSwarm(int antsNum, int sourcesNum)
{
    swarm.initialize(antsNum, sourcesNum);
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

void Scene::updateSwarmRenderInfo() 
{
    for (std::size_t i = 0, size = Ant::getAmount(); i < size; ++i) {
        updateObjectRenderInfo(i, swarm.ants_[i]);
    }

    // Updating sources objects    
    for (std::size_t i = 0, size = Source::getAmount(); i < size; ++i) {
        updateObjectRenderInfo(i + Ant::getAmount(), swarm.sources_[i]);
    }    
}  

void Scene::updateQuadTreeBuffer() 
{
    std::vector<Rectangle2d*> leafs;
    quadTree_.getLeafs(leafs);

    vec2 area = window_->getBounds().getMax();
    vec2 min, max;
    for (std::size_t i = 0, size = leafs.size(); i < size; ++i) {
        min = leafs[i]->getMin();
        max = leafs[i]->getMax();

        quadTreeRenderBuffer[i * 4] = min.x * area.x;
        quadTreeRenderBuffer[i * 4 + 1] = min.y * area.y;
        quadTreeRenderBuffer[i * 4 + 2] = (max.x - min.x) * area.x;
        quadTreeRenderBuffer[i * 4 + 3] = (max.y - min.y) * area.y;
    }
}

void Scene::update(const float alpha)
{
    processInput();

    swarm.update(alpha);
    updateSwarmRenderInfo();

    quadTree_.update(swarm.ants_);

    // Updating quad tree rendering data
    if (renderingQuadTree) {
        updateQuadTreeBuffer();
    }
    
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
 
    if (renderingQuadTree) {
        shaders_[1]->use();

        vec2 resolution = window_->getResolution();
        glUniform2f(glGetUniformLocation(shaders_[1]->getID(), "resolution"), resolution.x, resolution.y);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, quadTree_.getLeafsCnt() * 4 * sizeof(float), quadTreeRenderBuffer);

        glBindVertexArray(VAO_[1]);
        glDrawArrays(GL_POINTS, 0, quadTree_.getLeafsCnt());
    }

    glfwSwapBuffers(window_->glWindow_);
    glfwPollEvents();
}

void Scene::processInput()
{
    if (glfwGetKey(window_->glWindow_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_->glWindow_, true); 

    if (glfwGetKey(window_->glWindow_, GLFW_KEY_SPACE) == GLFW_PRESS)
        renderingQuadTree = !renderingQuadTree;

    vec2 velocity;
    if (glfwGetKey(window_->glWindow_, GLFW_KEY_UP) == GLFW_PRESS)
        velocity = {0.0f, 0.05f};
    if (glfwGetKey(window_->glWindow_, GLFW_KEY_RIGHT) == GLFW_PRESS)
        velocity = {0.05f, 0.0f};
    if (glfwGetKey(window_->glWindow_, GLFW_KEY_DOWN) == GLFW_PRESS)
        velocity = {0.0f, -0.05f};
    if (glfwGetKey(window_->glWindow_, GLFW_KEY_LEFT) == GLFW_PRESS)
        velocity = {-0.05f, 0.0f};

    for (std::size_t i = 0, size = swarm.ants_.size(); i < size; ++i)
        swarm.ants_[i]->pos_ += velocity;
}