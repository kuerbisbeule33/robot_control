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
#include <setpointwidget.h>
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

public:
    CodeEditor* editor;

};


#endif // MAINWIDGET_H
