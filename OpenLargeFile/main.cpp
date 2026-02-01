#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Эти штуки нужны чтобы QT создавал файл с историей, например, чтобы не вводить каждый раз
    //путь и имя файла
    QCoreApplication::setOrganizationName("SoftWare");
    QCoreApplication::setApplicationName("LogViewer");

    MainWindow w;
    w.show();
    return a.exec();
}
