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
#include <QFont>
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::MainWindow), port(nullptr)
{
    ui->setupUi(this);

    //Fenster anpassen
     QMainWindow::showMaximized();
     QMainWindow::setWindowTitle("Robot Control");

    //verbindungsmenü
    connectionMenu = menuBar()->addMenu("Connection");
    connectionMenu->setToolTipsVisible(true);
    QAction* set = new QAction("Settings");
    set->setIcon(QIcon("://settings.ico"));
    set->setIconVisibleInMenu(true);
    connect(set, SIGNAL(triggered()), this, SLOT(openSettings()));
    connectionMenu->addAction(set);
    connect(connectionMenu, SIGNAL(aboutToShow()), this, SLOT(search()));
    connectionMenu->addSeparator();

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
                 //port = new QSerialPort(infos.first().portName());
                 QAction* closeConnection = new QAction("disconnect");
                 closeConnection->setIcon(QIcon("://exit.ico"));
                 closeConnection->setIconVisibleInMenu(true);
                 connectionMenu->addAction(closeConnection);
                 connectionList.push_back(closeConnection);
                 connect(closeConnection, SIGNAL(triggered()), this, SLOT(disconnectPort()));
                 connectToPort(infos.first().portName());
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
    qDeleteAll(connectionList);
    connectionList.clear();

    //alle Ports auslesen
    const auto infos = QSerialPortInfo::availablePorts();
    if (infos.empty()){
        QAction* nothing = new QAction("no dvice available");
        nothing->setEnabled(false);
        connectionMenu->addAction(nothing);
        connectionList.push_back(nothing);
    }
    else{
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
            if (port != nullptr) {
                if (port->portName() == name){
                    detectedDevice->setIconVisibleInMenu(true);
                     detectedDevice->setIcon(QIcon("://connection.ico"));
                }
            }
            connectionMenu->addAction(detectedDevice);
            connectionList.push_back(detectedDevice);
            QString text = detectedDevice->text();
            connect(detectedDevice, &QAction::triggered, [this, text] {connectToPort(text);});
        }

        if (port != nullptr){
            QAction* closeConnection = new QAction("disconnect");
            closeConnection->setIcon(QIcon("://exit.ico"));
            closeConnection->setIconVisibleInMenu(true);
            connectionMenu->addAction(closeConnection);
            connectionList.push_back(closeConnection);
            connect(closeConnection, SIGNAL(triggered()), this, SLOT(disconnectPort()));
        }
    }
}

void MainWindow::connectToPort(QString portName){
    if (port == nullptr){
        port = new QSerialPort(portName);
        loadPortSettings();
        if (!port->open(QIODevice::WriteOnly)) {
            QMessageBox::information(0,"error", "Can't open Port");

        }
    }
    else {
        port->setPortName(portName);
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
    if (port != nullptr){
        delete port;
        port = nullptr;
    }
}

void MainWindow::openSettings(){
    settings = new SettingsDialog();
    settings->deleteLater();
    //connect(settings, QCloseEvent::Q, settings, QDialog::close);
    settings->exec();
}
