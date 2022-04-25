#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <codeeditor.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "pushbutton.h"

extern const double armLengthHorizontal;
extern const double armLengthVertical;
extern const double gripLength;

struct Point {
    double x;
    double y;
    double z;
};

double lawOfCosinus(double a, double b, double c);
Point angleToCoordinates(quint16 rotationAngle, quint16 tiltAngle, quint16 horizontalAngle);
void CoordinatesToAngle(Point point, quint16& rotationAngle, quint16& tiltAngle, quint16& horizontalAngle);


class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    QSpinBox* angle1;
    QSpinBox* angle2;
    QSpinBox* angle3;
    QSpinBox* angle4;
    PushButton* left1;
    PushButton* right1;
    PushButton* left2;
    PushButton* right2;
    PushButton* left3;
    PushButton* right3;
    PushButton* left4;
    PushButton* right4;
    QSpinBox* step;
    QPushButton* savePoint;
    QSlider* slider1;
    QSlider* slider2;
    QSlider* slider3;
    QSlider* slider4;
public:
    CodeEditor* editor;
};


#endif // MAINWIDGET_H
