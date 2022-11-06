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
    matModel_ = program_->uniformLocation("model");
    matView_ = program_->uniformLocation("view");
    matProjection_ = program_->uniformLocation("projection");
    matNormal_ = program_->uniformLocation("normalMatrix");
    lightPos_ = program_->uniformLocation("lightPos");

    timer_.start();

    // Release all
    program_->release();

    vao_.release();

    ibo_.release();
    nbo_.release();
    vbo_.release();

    // Prepare matrixes
    model_.setToIdentity();
    view_.setToIdentity();
    view_.lookAt(
            QVector3D(0.0f, 0.0f, 1.2f),   // Camera Position
            QVector3D(0.0f, 0.0f, 0.0f), // Point camera looks towards
            QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

    float aspect = 4.0f/3.0f;
    projection_.setToIdentity();
    projection_.perspective(
            60.0f,  // field of vision
            aspect,  // aspect ratio
            0.3f,     // near clipping plane
            1000.0f);  // far clipping plane

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

    QMatrix4x4 modelView = view_ * model_;
    QMatrix3x3 normalMatrix = modelView.normalMatrix();
    QVector3D light = QVector3D(-1.0f, 1.0f, 1.0f);

	// Update uniform value
	program_->setUniformValue(matModel_, model_);
	program_->setUniformValue(matView_, view_);
	program_->setUniformValue(matProjection_, projection_);
    program_->setUniformValue(matNormal_, normalMatrix);
    program_->setUniformValue(lightPos_, light);

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
