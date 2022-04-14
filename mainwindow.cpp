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
#include <QFile>
#include <QMessageBox>
#include <QIntValidator>
#include <QLineEdit>
#include <QMetaEnum>
#include <QString>
#include <QSignalMapper>
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Fenster anpassen
     QMainWindow::showMaximized();
     QMainWindow::setWindowTitle("Robot Control");

    //verbindungsmenü
    connectionMenu = menuBar()->addMenu("Connection");
    connectionMenu->setToolTipsVisible(true);
    QAction* set = new QAction("Settings");
    connect(set, SIGNAL(triggered()), this, SLOT(openSettings()));
    connectionMenu->addAction(set);
    connect(connectionMenu, SIGNAL(aboutToShow()), this, SLOT(search()));

    //wenn erlaubt, direkt verinden
    QFile file("config.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"error",file.errorString() + "\n\nno auto conneciton");
    }
    else{
        QString buffer;
        buffer = file.readLine();
        if (static_cast<Qt::CheckState>(QMetaEnum::fromType<Qt::CheckState>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))) == Qt::Checked) {
            const auto infos = QSerialPortInfo::availablePorts();
            if (infos.size() == 1){
                 port = new QSerialPort(infos.first());
                 loadPortSettings();
                 if (!port->open(QIODevice::WriteOnly)) {
                     QMessageBox::information(0,"error", "Can't open Port");
                 }
                 QAction* closeConnection = new QAction("disconnect");
                 connectionMenu->addAction(closeConnection);
                 connectionList.push_back(closeConnection);
                 connect(closeConnection, SIGNAL(triggered()), this, SLOT(disconnectPort()));
            }
        }
    }
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
    //connectionList.erase(beginn, end);
    qDeleteAll(connectionList);
    connectionList.clear();
    qDeleteAll(mapperList);
    mapperList.clear();

    //alle Ports auslesen
    const auto infos = QSerialPortInfo::availablePorts();
    //alle com ports filtern
    quint8 counter = 0;
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
        if (port->portName() == info.portName()){
            detectedDevice->hover();
        }
        connectionMenu->addAction(detectedDevice);
        connectionList.push_back(detectedDevice);

        //??????????
        QSignalMapper* sigMapper = new QSignalMapper(this);
        connect(sigMapper, SIGNAL(mappedString(Qstring)), this, SLOT(connectToPort(Qstring)));
        connect(detectedDevice, SIGNAL(triggered()), sigMapper, SLOT(map()));
        sigMapper->setMapping(detectedDevice, "test");
        //sigMapper->setMapping(detectedDevice, detectedDevice->text().toStdString().c_str());

        mapperList.push_back(sigMapper);

        ++counter;
    }

    if(counter == 0){
        QAction* nothing = new QAction("no dvice available");
        nothing->setEnabled(false);
        connectionMenu->addAction(nothing);
        connectionList.push_back(nothing);
    }
    else{
        QAction* closeConnection = new QAction("disconnect");
        connectionMenu->addAction(closeConnection);
        connectionList.push_back(closeConnection);
        connect(closeConnection, SIGNAL(triggered()), this, SLOT(disconnectPort()));
    }
}

void MainWindow::connectToPort(QString portName){
    qDebug() << "Port_open_" + portName + "_";
    if (port == nullptr){
            delete port;
    }
    port = new QSerialPort(portName);
    loadPortSettings();
    if (!port->open(QIODevice::WriteOnly)) {
        QMessageBox::information(0,"error", "Can't open Port");
    }
}

void MainWindow::loadPortSettings(){
    //serial config lesen
    QFile file("config.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"error",file.errorString() + "\n\ncant load settings, going on with default");
    }
    else{
        QString buffer;
        buffer = file.readLine();
        buffer = file.readLine();
        port->setBaudRate(static_cast<QSerialPort::BaudRate>(QMetaEnum::fromType<QSerialPort::BaudRate>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        buffer = file.readLine();
        port->setDataBits(static_cast<QSerialPort::DataBits>(QMetaEnum::fromType<QSerialPort::DataBits>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        buffer = file.readLine();
        port->setParity(static_cast<QSerialPort::Parity>(QMetaEnum::fromType<QSerialPort::Parity>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        buffer = file.readLine();
        port->setStopBits(static_cast<QSerialPort::StopBits>(QMetaEnum::fromType<QSerialPort::StopBits>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        buffer = file.readLine();
        port->setFlowControl(static_cast<QSerialPort::FlowControl>(QMetaEnum::fromType<QSerialPort::FlowControl>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        file.close();
    }
}

void MainWindow::disconnectPort(){
    port->flush();
    port->close();
    if (port == nullptr){
            delete port;
    }
}

void MainWindow::openSettings(){
    settings = new SettingsDialog();
    settings->deleteLater();
    //connect(settings, QCloseEvent::Q, settings, QDialog::close);
    settings->exec();
}
