#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QVector>
#include <QList>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qsignalmapper.h>
#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadPortSettings();

private:
    Ui::MainWindow *ui;
    SettingsDialog* settings;

    QMenu* connectionMenu;
    QList<QAction*> connectionList;
    QList<QSignalMapper*> mapperList;
    QSerialPort* port;

private slots:
    void search();
    void openSettings();
    void connectToPort(QString portName);
    void disconnectPort();
};
#endif // MAINWINDOW_H

//Test
