#include "MainWidget.h"
#include <QApplication>
#include "hnn/clifford13.h"

int main(int argc, char **argv)
{
    // Define basis elements
    auto e0 = Clifford13::basis(1);  // e0: index 1 (0b0001)
    auto e1 = Clifford13::basis(2);  // e1: index 2 (0b0010)

    // Compute e0 * e1
    auto e0e1 = e0 * e1;
    std::cout << "e0 * e1 = " << e0e1 << std::endl;  // Output: +1 e0e1

    // Compute e1 * e1
    auto e1e1 = e1 * e1;
    std::cout << "e1 * e1 = " << e1e1 << std::endl;  // Output: -1

    // Compute e0e1 * e1
    auto result = e0e1 * e1;
    std::cout << "e0e1 * e1 = " << result << std::endl;  // Output: -1 e0

    // Compute e0 * e0
    auto e0e0 = e0 * e0;
    std::cout << "e0 * e0 = " << e0e0 << std::endl;  // Output: +1

    std::cout << Clifford13::to_table_string() << std::endl;

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
