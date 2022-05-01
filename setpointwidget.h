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
    QLabel* labelGripper;
    QSlider* sliderGripper;
    QPushButton* savePoint;
    QPushButton* savePointGripper;

public:
    QTextBrowser* log;

signals:
    void newPosGcode(QString codeLine);
    void newGripperGcode(QString codeLine);

private slots:
    void newPosGcodeEmitted();
    void newGripperGcodeEmitted();

};

#endif // SETPOINTWIDGET_H
