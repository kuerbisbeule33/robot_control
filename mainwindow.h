#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QVector>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMenu* connectionMenu;
    QList<QAction*>connectionList;
    //QActionGroup* connectionGroup;
    //QVector<QAction*> connectionList;

private slots:
    void search();
};
#endif // MAINWINDOW_H

//Test
