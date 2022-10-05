#include <QApplication>
#include <QSurfaceFormat>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include "FractalWindow.h"

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

	FractalWindow fractalWindow;
	fractalWindow.setFormat(format);
	fractalWindow.resize(640, 640);

	QWidget *container = QWidget::createWindowContainer(&fractalWindow);
	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QWidget *window = new QWidget;

	QHBoxLayout *l = new QHBoxLayout(nullptr);
	QVBoxLayout *vl = new QVBoxLayout(nullptr);

	QLabel *iterationsLabel = new QLabel(QString("Number of iterations"));
	QSpinBox *iterationsSpinBox = new QSpinBox;
	iterationsSpinBox->setRange(100, 1000);
    iterationsSpinBox->setSingleStep(10);
    iterationsSpinBox->setValue(500);

	QObject::connect(iterationsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), &fractalWindow,
			qOverload<int>(&FractalWindow::setIterations));

	QLabel *radiusLabel = new QLabel(QString("Radius of fractal"));
	QDoubleSpinBox *radiusSpinBox = new QDoubleSpinBox;
	radiusSpinBox->setRange(4.0, 20.0);
    radiusSpinBox->setSingleStep(0.5);
    radiusSpinBox->setValue(10.0);

	QObject::connect(radiusSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), &fractalWindow,
			qOverload<float>(&FractalWindow::setRadius));

	QLabel *powerLabel = new QLabel(QString("Gradient of colors"));
	QDoubleSpinBox *powerSpinBox = new QDoubleSpinBox;
	powerSpinBox->setRange(0.1, 0.25);
    powerSpinBox->setSingleStep(0.05);
    powerSpinBox->setValue(0.2);

	QObject::connect(powerSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), &fractalWindow,
			qOverload<float>(&FractalWindow::setPower));

	l->addWidget(container);
	l->addLayout(vl);

	vl->addWidget(iterationsLabel);
	vl->addWidget(iterationsSpinBox);
	vl->addWidget(radiusLabel);
	vl->addWidget(radiusSpinBox);
	vl->addWidget(powerLabel);
	vl->addWidget(powerSpinBox);
	vl->addStretch(1);

	window->setLayout(l);
	window->resize(1000, 900);
	window->show();

	fractalWindow.setAnimated(true);

	return app.exec();
}