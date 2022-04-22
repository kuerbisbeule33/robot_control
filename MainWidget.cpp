#include "MainWidget.h"
#include <QtMath>

const double armLengthHorizontal = 1.0;
const double armLengthVertical = 1.0;
const double gripLength = 1.0;

double lawOfCosinus(double a, double b, double c){
    return qAcos((a*a + b*b - c*c) / (2*a*b));
}

Point angleToCoordinates(quint16 rotationAngle, quint16 tiltAngle, quint16 horizontalAngle){
    //x nach rechts
    //y nach oben
    //z nach vorne
    Point point;
    double rotation = static_cast<double>(rotationAngle);
    double tilt = static_cast<double>(tiltAngle);
    double horizontal = static_cast<double>(horizontalAngle);
    double ditanzXZ = qCos(tilt) * armLengthVertical + qCos(180-tilt-horizontal) * armLengthHorizontal + gripLength;
    point.x = qCos(rotation) * ditanzXZ;
    point.y = qSin(tilt) * armLengthVertical - qSin(180-tilt-horizontal) * armLengthHorizontal;
    point.z = -qSin(rotation) * ditanzXZ;
    return point;
}

void CoordinatesToAngle(Point point, quint16& rotationAngle, quint16& tiltAngle, quint16& horizontalAngle){
    //2 teile der Koordinaten von https://appliedgo.net/roboticarm/
    //x nach rechts
    //y nach oben
    //z nach vorne
    rotationAngle = static_cast<quint16>(qAtan2(-point.z, point.x));
    double distOnGround = qSqrt(point.x*point.x + point.z*point.z);
    double dist2Dimensions = qSqrt(point.y*point.y + distOnGround*distOnGround);
    quint16 helpAngle1 = qAtan2(point.y, distOnGround);
    quint16 helpAngle2 = static_cast<quint16>(lawOfCosinus(dist2Dimensions, armLengthVertical, armLengthHorizontal));
    tiltAngle = helpAngle1 + helpAngle2;
    horizontalAngle = static_cast<quint16>(lawOfCosinus(armLengthVertical, armLengthHorizontal, dist2Dimensions));
}

MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{
    slider1 = new QSlider(Qt::Orientation::Horizontal);
    slider1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    angle1 = new QSpinBox;
    left1 = new QPushButton(QIcon("://left.png"), "");
    right1 = new QPushButton(QIcon("://right.png"), "");
    slider1HBox = new QHBoxLayout;
    slider1HBox->addWidget(slider1);
    slider1HBox->addWidget(angle1);
    slider1HBox->addWidget(left1);
    slider1HBox->addWidget(right1);

    slider2 = new QSlider(Qt::Orientation::Horizontal);
    slider2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    angle2 = new QSpinBox;
    left2 = new QPushButton(QIcon("://left.png"), "");
    right2 = new QPushButton(QIcon("://right.png"), "");
    slider2HBox = new QHBoxLayout;
    slider2HBox->addWidget(slider2);
    slider2HBox->addWidget(angle2);
    slider2HBox->addWidget(left2);
    slider2HBox->addWidget(right2);

    slider3 = new QSlider(Qt::Orientation::Horizontal);
    slider3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    angle3 = new QSpinBox;
    left3 = new QPushButton(QIcon("://left.png"), "");
    right3 = new QPushButton(QIcon("://right.png"), "");
    slider3HBox = new QHBoxLayout;
    slider3HBox->addWidget(slider3);
    slider3HBox->addWidget(angle3);
    slider3HBox->addWidget(left3);
    slider3HBox->addWidget(right3);

    slider4 = new QSlider(Qt::Orientation::Horizontal);
    slider4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    angle4 = new QSpinBox;
    left4 = new QPushButton(QIcon("://left.png"), "");
    right4 = new QPushButton(QIcon("://right.png"), "");
    slider4HBox = new QHBoxLayout;
    slider4HBox->addWidget(slider4);
    slider4HBox->addWidget(angle4);
    slider4HBox->addWidget(left4);
    slider4HBox->addWidget(right4);

    step = new QSpinBox;
    savePoint = new QPushButton("save Point in File");
    savePoint->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    otherHBox = new QHBoxLayout;
    otherHBox->addWidget(step);
    otherHBox->addWidget(savePoint);


    sliderVbox = new QVBoxLayout;
    sliderVbox->addLayout(slider1HBox);
    sliderVbox->addLayout(slider2HBox);
    sliderVbox->addLayout(slider3HBox);
    sliderVbox->addLayout(slider4HBox);
    sliderVbox->addLayout(otherHBox);

    editor = new CodeEditor;
    globalHbox = new QHBoxLayout;
    globalHbox->addLayout(sliderVbox);
    globalHbox->addWidget(editor);
    setLayout(globalHbox);

}
