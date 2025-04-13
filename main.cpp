#include "MainWidget.h"
#include <QApplication>
#include "hnn/clifford13.h"
#include "hnn/clifford31.h"

int main(int argc, char **argv)
{
    // std::cout << Clifford13::to_table_string() << std::endl;
    // std::cout << Clifford13::to_table_string_tex1() << std::endl;
    // std::cout << Clifford13::to_table_string_tex2() << std::endl;
    std::cout << Clifford13::to_table_string_tex() << std::endl;
    std::cout << Clifford31::to_table_string_tex() << std::endl;

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
