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
    sceneObj.quadTree_ = QuadTree(window);

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

void Scene::genAnts(GLint num)
{   
    using namespace Swarm;

    vec2 resolution = window_->getResolution();
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(200.0f + rand() % (int)resolution.x / 5,
                        200.0f + rand() % (int)resolution.y / 5);
        vec3 color = vec3(255.0f, 255.0f, 255.0f);
        Ant* ant = new Ant(pos, 25.0f, 2.0f, ANT_SIZE, color);
        ants.push_back(ant);
    }
}

void Scene::genSources(GLint num)
{
    using namespace Swarm;

    vec2 resolution = window_->getResolution();
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(rand() % (int)resolution.x, rand() % (int)resolution.y);
        vec3 color = vec3((int)pos.x % 255, (int)pos.y % 255, (int)rand() % 255);
        sources.push_back(new Swarm::Source(pos, 2.0f, SOURCE_SIZE, color));
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

void Scene::updateQuadTreeBuffer() {
    std::vector<Rectangle2d*> leafs;
    quadTree_.getLeafs(leafs);

    vec2 min, max;
    for (std::size_t i = 0, size = leafs.size(); i < size; ++i) {
        min = leafs[i]->getMin();
        max = leafs[i]->getMax();

        quadTreeRenderBuffer[i * 4] = min.x;
        quadTreeRenderBuffer[i * 4 + 1] = min.y;
        quadTreeRenderBuffer[i * 4 + 2] = max.x - min.x;
        quadTreeRenderBuffer[i * 4 + 3] = max.y - min.y;
    }
}

void Scene::update(const float alpha)
{
    processInput();

    // Updating ants objects
    for (std::size_t i = 0, size = Swarm::Ant::getAmount(); i < size; ++i) {
         ants[i]->update(alpha);
        updateObjectRenderInfo(i, ants[i]);
    }

    // Updating sources objects    
    for (std::size_t i = 0, size = Swarm::Source::getAmount(); i < size; ++i) {
        sources[i]->update(alpha);
        updateObjectRenderInfo(i + Swarm::Ant::getAmount(), sources[i]);
    }

    quadTree_.update(ants);
    // Updating quad tree rendering data
    if (renderingQuadTree) {
        updateQuadTreeBuffer();
    }
    
    objectsAmount_ = Swarm::Ant::getAmount() + Swarm::Source::getAmount();
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
        velocity = {0.0f, 5.0f};
    if (glfwGetKey(window_->glWindow_, GLFW_KEY_RIGHT) == GLFW_PRESS)
        velocity = {5.0f, 0.0f};
    if (glfwGetKey(window_->glWindow_, GLFW_KEY_DOWN) == GLFW_PRESS)
        velocity = {0.0f, -5.0f};
    if (glfwGetKey(window_->glWindow_, GLFW_KEY_LEFT) == GLFW_PRESS)
        velocity = {-5.0f, 0.0f};

    for (std::size_t i = 0, size = ants.size(); i < size; ++i)
        ants[i]->pos_ += velocity;
}