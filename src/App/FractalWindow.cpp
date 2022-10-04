#include "FractalWindow.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QScreen>
#include <QDebug>

#include <array>

namespace
{

constexpr std::array<GLfloat, 8u> vertices = {
	-1.0f, -1.0f,
	-1.0f, 1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,
};
constexpr std::array<GLuint, 6u> indices = {
	0, 1, 2,
	1, 2, 3,
};

constexpr float SCALE = 0.7f;
constexpr float CONST_SIZE = 640.0f;

}// namespace

void FractalWindow::init()
{
	// Configure shaders
	program_ = std::make_unique<QOpenGLShaderProgram>(this);
	program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/fractal.vs");
	program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
									  ":/Shaders/fractal.fs");
	program_->link();

	// Create VAO object
	vao_.create();
	vao_.bind();

	// Create VBO
	vbo_.create();
	vbo_.bind();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(GLfloat)));

	// Create IBO
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	ibo_.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(GLuint)));

	// Bind attributes
	program_->bind();

	program_->enableAttributeArray(0);
	program_->setAttributeBuffer(0, GL_FLOAT, 0, 2, static_cast<int>(2 * sizeof(GLfloat)));

	fractalColor_1_ = program_->uniformLocation("in_col1");
	fractalColor_2_ = program_->uniformLocation("in_col2");
	maxIterations_ = program_->uniformLocation("max_it");
	matrix_ = program_->uniformLocation("transform");

	// Release all
	program_->release();

	vao_.release();

	ibo_.release();
	vbo_.release();

	// Uncomment to enable depth test and face culling
	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);

	// Clear all FBO buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FractalWindow::render()
{
	// Configure viewport
	const auto retinaScale = devicePixelRatio();
	glViewport(0, 0, static_cast<GLint>(width() * retinaScale),
			   static_cast<GLint>(height() * retinaScale));

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate MVP matrix
	// QMatrix4x4 matrix;
	/*
	matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	matrix.translate(0, 0, -2);
	const auto angle = 100.0 * static_cast<double>(frame_) / screen()->refreshRate();
	matrix.rotate(static_cast<float>(angle), rotationAxis_);
	*/

	QVector3D in_col1 = {0.51f, 0.21f, 0.82f};
	QVector3D in_col2 = {0.13f, 0.41f, 0.83f};
	int max_it = 500;

	// Bind VAO and shader program
	program_->bind();
	vao_.bind();

	// Update uniform value
	program_->setUniformValue(fractalColor_1_, in_col1);
	program_->setUniformValue(fractalColor_2_, in_col2);
	program_->setUniformValue(maxIterations_, max_it);
	program_->setUniformValue(matrix_, transform_);

	// Draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Release VAO and shader program
	vao_.release();
	program_->release();

	// Increment frame counter
	++frame_;
}

void FractalWindow::mousePressEvent(QMouseEvent * e)
{
	QVector2D mousePressPosition = QVector2D(e->localPos());
	QString x = QString::number(-mousePressPosition.x() * 2.0f / CONST_SIZE);
	QString y = QString::number(-(CONST_SIZE - mousePressPosition.y()) * 2.0f / CONST_SIZE);
	qDebug() << x << "," << y;
	transform_.translate(mousePressPosition.x() * 2.0f / CONST_SIZE,
								(CONST_SIZE - mousePressPosition.y()) * 2.0f / CONST_SIZE);
	transform_.scale(SCALE, SCALE);
	transform_.translate(-mousePressPosition.x() * 2.0f / CONST_SIZE,
								-(CONST_SIZE - mousePressPosition.y()) * 2.0f / CONST_SIZE);
}
/*
void FractalWindow::wheelEvent(QWheelEvent * e)
{
	QPoint num_degrees = e->angleDelta();
	

}
*/
/*
void FractalWindow::mouseReleaseEvent(QMouseEvent * e)
{
	const auto diff = QVector2D(e->localPos()) - mousePressPosition_;
	rotationAxis_ = QVector3D(diff.y(), diff.x(), 0.0).normalized();
}
*/