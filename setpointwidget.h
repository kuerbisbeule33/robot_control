#ifndef SETPOINTWIDGET_H
#define SETPOINTWIDGET_H

#include <QWidget>
#include <QString>
#include <QTextBrowser>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

class SetPointWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SetPointWidget(QWidget *parent = nullptr);

public:
    QComboBox* gCode;
    QSpinBox* xPos;
    QSpinBox* yPos;
    QSpinBox* zPos;
    QLabel* labelGripper1;
    QSlider* sliderGripper1;
    QPushButton* savePoint;
    QPushButton* savePointGripper1;
    QSlider* sliderVertical2;
    QSlider* sliderHorizontal3;
    QSlider* sliderRotation4;

public:
    QTextBrowser* log;

signals:
    void newPosGcode(QString codeLine);
    void newGripperGcode(QString codeLine);

private slots:
    void newPosGcodeEmitted();
    void newGripperGcodeEmitted();
    void changeXYZpos();
    void changeAnglePos();

};

#endif // SETPOINTWIDGET_H
