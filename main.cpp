#include <QApplication>
#include <QTranslator>
#include <QString>

#include "mainwindow.h"

int main(int argc, char *argv[])
{ 
    QString locale = QLocale::system().name().section('_', 0, 0);
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(("chimie_" + locale));
    a.installTranslator(&translator);

    MainWindow w;

    w.show();

    return a.exec();
}

