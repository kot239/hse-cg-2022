#pragma once

#include <Base/GLWindow.hpp>

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>

#include <memory>

class FractalWindow final : public fgl::GLWindow
{

public:
	void init() override;
	void render() override;

protected:
	void mousePressEvent(QMouseEvent * e) override;
	//void wheelEvent(QWheelEvent * e) override;
	//void mouseReleaseEvent(QMouseEvent * e) override;

private:
	GLint fractalColor_1_ = -1;
	GLint fractalColor_2_ = -1;
	GLint maxIterations_ = -1;
	GLint matrix_ = -1;

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;

	size_t frame_ = 0;
	QMatrix4x4 transform_;

	QVector3D rotationAxis_{0., 1., 0.};
};
