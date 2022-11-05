#include "GLLoader.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QScreen>
#include <QDebug>
#include <QString>

#include <array>

namespace {

    QString modelFilepath("../../../resources/pjanic.dae");

} // namespace

void GLLoader::init()
{
    // Configure shaders
    program_ = std::make_unique<QOpenGLShaderProgram>(this);
    program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/lightning.vs");
    program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                      ":/Shaders/lightning.fs");

    program_->link();

    // Load model
    ModelLoader model;

    if (!model.load(modelFilepath)) {
        return;
    }

    QVector<float> *vertices_;
    QVector<float> *normals_;
    QVector<std::size_t> *indices_;

    model.getBufferData(&vertices_, &normals_, &indices_);
    rootNode_ = model.getNode();

    // Create VAO object
    vao_.create();
    vao_.bind();

    // Create VBO
    vbo_.create();
    vbo_.bind();
    vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo_.allocate(&(*vertices_)[0], static_cast<int>(vertices_->size() * sizeof(float)));

    // Create NBO
    nbo_.create();
    nbo_.bind();
    nbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    nbo_.allocate(&(*normals_)[0], static_cast<int>(normals_->size() * sizeof(float)));

    // Create IBO
    ibo_.create();
    ibo_.bind();
    ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo_.allocate(&(*indices_)[0], static_cast<int>(indices_->size() * sizeof(std::size_t)));

    // Bind attributes
    program_->bind();

    vbo_.bind();
    program_->enableAttributeArray(0);
    program_->setAttributeBuffer(0, GL_FLOAT, 0, 3, static_cast<int>(3 * sizeof(GLfloat)));

    nbo_.bind();
    program_->enableAttributeArray(1);
    program_->setAttributeBuffer(1, GL_FLOAT, 0, 3, static_cast<int>(3 * sizeof(GLfloat)));

    // Declare uniform variables
    /*
    fractalColor_1_ = program_->uniformLocation("in_col1");
    fractalColor_2_ = program_->uniformLocation("in_col2");
    windowHeight_ = program_->uniformLocation("height");
    windowWidth_ = program_->uniformLocation("width");
    maxIterations_ = program_->uniformLocation("max_it");
    fractalRadius_ = program_->uniformLocation("radius");
    fractalPower_ = program_->uniformLocation("power");
    matrix_ = program_->uniformLocation("transform");
     */


    timer_.start();

    // Release all
    program_->release();

    vao_.release();

    ibo_.release();
    nbo_.release();
    vbo_.release();

    // Uncomment to enable depth test and face culling
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    // Clear all FBO buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLLoader::render()
{
    // Configure viewport
    /*
	const auto retinaScale = devicePixelRatio();
	glViewport(0, 0, static_cast<GLint>(width() * retinaScale),
			   static_cast<GLint>(height() * retinaScale));
    */

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind VAO and shader program
	program_->bind();
	vao_.bind();

	// Update uniform value
    /*
	program_->setUniformValue(fractalColor_1_, in_col1);
	program_->setUniformValue(fractalColor_2_, in_col2);
	program_->setUniformValue(windowHeight_, height());
	program_->setUniformValue(windowWidth_, width());
	program_->setUniformValue(maxIterations_, max_it_);
	program_->setUniformValue(fractalRadius_, radius_);
	program_->setUniformValue(fractalPower_, power_);
	program_->setUniformValue(matrix_, transform_);
     */

	// Draw
        drawNode(rootNode_.data());

	// Release VAO and shader program
	vao_.release();
	program_->release();

    frames_++;
    countFPS();
}

void GLLoader::drawNode(const Node *node) {
    for (auto & mesh : node->meshes) {
        glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT,
                       (const void*)(mesh->indexOffset * sizeof(unsigned int)));
    }

    for(auto & ch_node : node->nodes)
        drawNode(&ch_node);
}

void GLLoader::countFPS()
{
    float current_time = timer_.elapsed() * 0.001f;
    fps_label_->setText("fps: " + QString::number(frames_));
    if (current_time - last_time_ > 1.0f)
    {
        last_time_ = current_time;
        frames_ = 0;
    }
}
