#include "mainwindow.h"

#include <QtGui>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication pEdit(argc, argv);
    MainWindow window;
    window.resize(640, 512);
    window.show();

    return pEdit.exec();
}
