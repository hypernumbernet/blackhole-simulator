#include "mainwidget.h"

#include <QApplication>
#include <QGLFormat>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("blackhole-simulator-2");
    app.setApplicationVersion("2.0 beta");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(0);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);

    MainWidget widget;
    widget.resize(1200, 800);
    widget.show();

    return app.exec();
}
