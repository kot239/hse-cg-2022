#include <QApplication>
#include <QSurfaceFormat>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QLabel>
#include <QSpinBox>

#include "GLLoader.h"

namespace
{
constexpr auto g_sampels = 16;
constexpr auto g_gl_major_version = 3;
constexpr auto g_gl_minor_version = 3;
}// namespace

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	QSurfaceFormat format;
	format.setSamples(g_sampels);
	format.setVersion(g_gl_major_version, g_gl_minor_version);
	format.setProfile(QSurfaceFormat::CoreProfile);

    QLabel *FPSLabel = new QLabel();

	// Add OpenGL window
    GLLoader *glWindow = new GLLoader(FPSLabel);
    glWindow->setFormat(format);
    glWindow->resize(640, 640);

	// Add container that contains OpenGL window
	QWidget *container = QWidget::createWindowContainer(glWindow);
	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Create window of app
	QWidget *window = new QWidget;

	// Make grid
	QHBoxLayout *l = new QHBoxLayout(nullptr);
	QVBoxLayout *vl = new QVBoxLayout(nullptr);

    // Add container and layout:  [container | vl]
    l->addWidget(container);
    l->addLayout(vl);

    QRadioButton *buttonVert = new QRadioButton("Vertex lightning");
    QRadioButton *buttonFrag = new QRadioButton("Fragment lightning");
    buttonVert->setChecked(true);

    QObject::connect(buttonVert, &QRadioButton::clicked, glWindow,
                     &GLLoader::setIsVertexLightning);
    QObject::connect(buttonFrag, &QRadioButton::clicked, glWindow,
                     &GLLoader::setIsFragmentLightning);

    vl->addWidget(buttonVert);
    vl->addWidget(buttonFrag);
    vl->addWidget(FPSLabel);
	vl->addStretch(1);

	// Show window
	window->setLayout(l);
	window->resize(1000, 900);
	window->show();

    glWindow->setAnimated(true);

	return app.exec();
}