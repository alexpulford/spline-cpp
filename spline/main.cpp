#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    QSurfaceFormat format;
    format.setSamples(16);
    format.setOption(QSurfaceFormat::DebugContext);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(1);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    a.setStyleSheet(".QFrame { background-color: palette(base); border: 1px solid #1b1b1b; }");
    a.setStyleSheet(".QScrollArea { background-color: palette(base); border: 1px solid #1b1b1b; }");
    MainWindow w;

    w.setStatus("Loading...");
    w.show();
    return a.exec();
}
