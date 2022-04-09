#include "mainwindow.h"
#include <QApplication>
#include "MainWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MainWidget* mWidget = new MainWidget();
    w.setCentralWidget(mWidget);
    w.show();
    return a.exec();
}
