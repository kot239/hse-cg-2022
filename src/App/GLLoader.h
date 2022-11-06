#pragma once

#include <Base/GLWindow.hpp>
#include "ModelLoader.h"

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

class GLLoader final : public fgl::GLWindow
{

public:
    explicit GLLoader(QLabel* fps_label): fps_label_(fps_label) {};

	void init() override;
	void render() override;

    /*
protected:
	void mousePressEvent(QMouseEvent * e) override;
	void wheelEvent(QWheelEvent * e) override;
*/

private:
    void drawNode(const Node* node);
    void countFPS();

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
    QOpenGLBuffer nbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;

    QSharedPointer<Node> rootNode_;

    GLint matModel_ = -1;
    GLint matView_ = -1;
    GLint matProjection_ = -1;
    GLint matNormal_ = -1;
    GLint lightPos_ = -1;

    QMatrix4x4 model_, view_, projection_;

    int frames_ = 0;
    float last_time_ = 0.0f;
    QElapsedTimer timer_;

    QLabel* fps_label_;
};
