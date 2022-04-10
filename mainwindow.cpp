#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QDebug>
#include <QIterator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //usb verbindungsmenü
    connectionMenu = menuBar()->addMenu(tr("&Connection"));
    connectionMenu->setToolTipsVisible(true);
    connect(connectionMenu, QMenu::aboutToShow, this, MainWindow::search);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::search(){
    //alle actions löschen
    for (const auto& i : connectionList){
        connectionMenu->removeAction(i);
    }
    connectionList.clear();

    //alle Ports auslesen
    const auto infos = QSerialPortInfo::availablePorts();
    //alle com ports filtern
    for (const QSerialPortInfo &info : infos) {
        QString deviceInfo = (info.description().isEmpty() ? "" :   QObject::tr("Description: ") + info.description() + "\n")
                           + (info.manufacturer().isEmpty() ? "" :  QObject::tr("Manufacturer: ") + info.manufacturer() + "\n")
                           + (info.serialNumber().isEmpty() ? "" :  QObject::tr("Serial number: ") + info.serialNumber() + "\n")
                           + (info.hasVendorIdentifier() ? "" :     QObject::tr("Vendor Identifier: ") + QString::number(info.vendorIdentifier(), 16) + "\n")
                           + (info.hasProductIdentifier() ? "" :    QObject::tr("Product Identifier: ") + QString::number(info.productIdentifier(), 16) + "\n");

        //gefundene Verbidnung als auswahl eintragen
        QString name = info.portName();
        QAction* detectedDevice = new QAction(name);
        detectedDevice->setToolTip(deviceInfo);
        //connectionGroup->addAction(detectedDevice);
        connectionMenu->addAction(detectedDevice);
        connectionList.push_back(detectedDevice);

        qDebug() << deviceInfo << "\n";

    }
}
