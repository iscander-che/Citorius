#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::alert(&w, 5000);

    return a.exec();
}
