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
#include "settingsdialog.h"
#include "MainWidget.h"

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
    Ui::MainWindow* ui;
    MainWidget* mainWidget;
    SettingsDialog* settings;
    QMenu* connectionMenu;
    QList<QAction*> connectionList;
    QSerialPort* port;
    QString curFile;

private slots:
    void search();
    void openSettings();
    void connectToPort(QString portName);
    void disconnectPort();

    void newFile();
    bool save();
    bool saveAs();
    void open();
    void documentWasModified();

public:

    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H

//Test
