#include "MainWidget.h"
#include <QApplication>
#include "hnn/clifford13.h"

int main(int argc, char **argv)
{
    std::cout << Clifford13::to_table_string() << std::endl;
    std::cout << Clifford13::to_table_string_tex1() << std::endl;
    std::cout << Clifford13::to_table_string_tex2() << std::endl;

    Clifford13::CoeffArray coeffs = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
                                     9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
    Clifford13 mv(coeffs);
    Clifford13 rev = mv.reverse();
    Clifford13 conj = mv.conjugation();

    // Print original and reversed coefficients for verification
    for (uint i = 0; i < 16; ++i) {
        uint o = Clifford13::order[i];
        printf("%2u: %8s: Original = %4.1f, Reversed = %5.1f, Conjugated = %5.1f\n",
               i, Clifford13::labels[o], mv.get_coeff(o), rev.get_coeff(o), conj.get_coeff(o));
    }

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
