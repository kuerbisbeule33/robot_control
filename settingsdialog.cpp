/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QIntValidator>
#include <QLineEdit>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFile>
#include <QMetaEnum>
#include <QString>
#include <QRegularExpression>


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsDialog)
{
    m_ui->setupUi(this);

    this->setWindowTitle("Connection Settings");
    this->setWindowIcon(QIcon("settings.ico"));

    m_ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    m_ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    m_ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    m_ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    m_ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    m_ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    m_ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    m_ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);

    m_ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    m_ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    m_ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    m_ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    m_ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    m_ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    m_ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
    m_ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    m_ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    m_ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    m_ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

    //serial config lesen
    QFile file("config.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"error",file.errorString() + "\n\ngoing on with default");
        m_ui->baudRateBox->setCurrentIndex(3);
        m_ui->dataBitsBox->setCurrentIndex(3);
        m_ui->parityBox->setCurrentIndex(0);
        m_ui->dataBitsBox->setCurrentIndex(0);
        m_ui->flowControlBox->setCurrentIndex(0);
    }
    else{
        QString buffer;
        buffer = file.readLine();
        m_ui->autoConnectBox->setCheckState(static_cast<Qt::CheckState>(QMetaEnum::fromType<Qt::CheckState>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", ""))));
        buffer = file.readLine();
        auto baudRate = static_cast<QSerialPort::BaudRate>(QMetaEnum::fromType<QSerialPort::BaudRate>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", "")));
        for (quint8 i = 0; i < m_ui->baudRateBox->count(); ++i){
            if (m_ui->baudRateBox->itemData(i) == baudRate) {m_ui->baudRateBox->setCurrentIndex(i);}
        }
        buffer = file.readLine();
        auto dataBits = static_cast<QSerialPort::DataBits>(QMetaEnum::fromType<QSerialPort::DataBits>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", "")));
        for (quint8 i = 0; i < m_ui->dataBitsBox->count(); ++i){
            if (m_ui->dataBitsBox->itemData(i) == dataBits) {m_ui->dataBitsBox->setCurrentIndex(i);}
        }
        buffer = file.readLine();
        auto parity = static_cast<QSerialPort::Parity>(QMetaEnum::fromType<QSerialPort::Parity>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", "")));
        for (quint8 i = 0; i < m_ui->parityBox->count(); ++i){
            if (m_ui->parityBox->itemData(i) == parity) {m_ui->parityBox->setCurrentIndex(i);}
        }
        buffer = file.readLine();
        auto stopBits = static_cast<QSerialPort::StopBits>(QMetaEnum::fromType<QSerialPort::StopBits>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", "")));
        for (quint8 i = 0; i < m_ui->stopBitsBox->count(); ++i){
            if (m_ui->stopBitsBox->itemData(i) == stopBits) {m_ui->stopBitsBox->setCurrentIndex(i);}
        }
        buffer = file.readLine();
        auto flowControl = static_cast<QSerialPort::FlowControl>(QMetaEnum::fromType<QSerialPort::FlowControl>().keyToValue(buffer.split(" ").at(2).toUtf8().replace("\n", "")));
        for (quint8 i = 0; i < m_ui->flowControlBox->count(); ++i){
            if (m_ui->flowControlBox->itemData(i) == flowControl) {m_ui->flowControlBox->setCurrentIndex(i);}
        }
        file.close();
    }
}

SettingsDialog::~SettingsDialog()
{
    //in datei speichern
    QFile file("config.txt");
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0,"error",file.errorString() + "\n\ncant save settings");
    }
    else{
        QTextStream out(&file);
        out << "AutoConnect = " << QString::fromUtf8(QMetaEnum::fromType<Qt::CheckState>().valueToKey(static_cast<int>(m_ui->autoConnectBox->checkState()))) << "\n"
            << "BaudRate = " <<  QString::fromUtf8(QMetaEnum::fromType<QSerialPort::BaudRate>().valueToKey(m_ui->baudRateBox->itemData(m_ui->baudRateBox->currentIndex()).toInt())) << "\n"\
            << "DataBits = " << QString::fromUtf8(QMetaEnum::fromType<QSerialPort::DataBits>().valueToKey(m_ui->dataBitsBox->itemData(m_ui->dataBitsBox->currentIndex()).toInt())) << "\n"\
            << "Parity = " << QString::fromUtf8(QMetaEnum::fromType<QSerialPort::Parity>().valueToKey(m_ui->parityBox->itemData(m_ui->parityBox->currentIndex()).toInt())) << "\n"\
            << "StopBits = " << QString::fromUtf8(QMetaEnum::fromType<QSerialPort::StopBits>().valueToKey(m_ui->stopBitsBox->itemData(m_ui->stopBitsBox->currentIndex()).toInt())) << "\n"\
            << "FlowControl = " << QString::fromUtf8(QMetaEnum::fromType<QSerialPort::FlowControl>().valueToKey(m_ui->flowControlBox->itemData(m_ui->flowControlBox->currentIndex()).toInt())) << "\n";
        file.flush();
        file.close();
    }

    delete m_ui;
}

