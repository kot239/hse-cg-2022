#pragma once

#include <Base/GLWindow.hpp>

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>
#include <QElapsedTimer>
#include <QLabel>

#include <memory>

class FractalWindow final : public fgl::GLWindow
{

public:
    explicit FractalWindow(QLabel* fps_label): fps_label_(fps_label) {};

	void init() override;
	void render() override;

	void setIterations(int it);
	void setRadius(float radius);
	void setPower(float power);

protected:
	void mousePressEvent(QMouseEvent * e) override;
	void wheelEvent(QWheelEvent * e) override;

private:
    void countFPS();

	GLint fractalColor_1_ = -1;
	GLint fractalColor_2_ = -1;

	GLint windowHeight_ = -1;
	GLint windowWidth_ = -1;

	GLint maxIterations_ = -1;
	GLint fractalRadius_ = -1;
	GLint fractalPower_ = -1;

	GLint matrix_ = -1;

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;

    int frames_ = 0;
    float last_time_ = 0.0f;
    QElapsedTimer timer_;

    QLabel* fps_label_;

	int max_it_ = 100;
	float radius_ = 20.0f;
	float power_ = 0.2f;
	QMatrix4x4 transform_;
};
