#include "mainwindow.h"
#include "syntaxlighter.h"
#include <QtGui>
#include <QApplication>
#include <iostream>
#include <cstdlib>

#include "syntaxlighter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.resize(640, 512);
    window.show();

    return a.exec();
}
