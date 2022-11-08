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
#include <unordered_set>

class GLLoader final : public fgl::GLWindow
{

public:
    explicit GLLoader(QLabel* fps_label): fps_label_(fps_label) {};

	void init() override;
	void render() override;

    void setIsVertexLightning();
    void setIsFragmentLightning();

protected:
	void keyPressEvent(QKeyEvent * e) override;
    void keyReleaseEvent(QKeyEvent * e) override;
	void wheelEvent(QWheelEvent * e) override;

private:
    void drawNode(const Node* node);
    void processEvents();
    void countFPS();

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
    QOpenGLBuffer nbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;

    QSharedPointer<Node> rootNode_;

    std::unordered_set<int> pressedKeys_;

    GLint matModel_ = -1;
    GLint matView_ = -1;
    GLint matProjection_ = -1;
    GLint matNormal_ = -1;
    GLint lightPos_ = -1;
    GLint modelTime_ = -1;
    GLint lightColor_ = -1;
    GLint isVertexLightning_ = -1;

    QMatrix4x4 model_, view_, projection_;
    QVector3D cameraPos_ = QVector3D(0.0f, 0.0f, 1.2f);

    bool flagIsVertexLightning_ = true;

    int frames_ = 0;
    int total_frames_ = 0;
    float last_time_ = 0.0f;
    QElapsedTimer timer_;

    QLabel* fps_label_;
};
