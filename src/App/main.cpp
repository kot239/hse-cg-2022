#include <QApplication>
#include <QSurfaceFormat>

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

	FractalWindow window;
	window.setFormat(format);
	window.resize(640, 640);
	window.show();

	window.setAnimated(true);

	return app.exec();
}