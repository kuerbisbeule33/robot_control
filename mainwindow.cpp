#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectionMenu = menuBar()->addMenu(tr("&Connection"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

