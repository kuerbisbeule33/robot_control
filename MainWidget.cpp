#include "MainWidget.h"
#include <QtMath>
#include "pushbutton.h"

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
    double distOnGround = qSqrt(point.x*point.x + point.z*point.z) - gripLength;
    double dist2Dimensions = qSqrt(point.y*point.y + distOnGround*distOnGround);
    quint16 helpAngle1 = qAtan2(point.y, distOnGround);
    quint16 helpAngle2 = static_cast<quint16>(lawOfCosinus(dist2Dimensions, armLengthVertical, armLengthHorizontal));
    tiltAngle = helpAngle1 + helpAngle2;
    horizontalAngle = static_cast<quint16>(lawOfCosinus(armLengthVertical, armLengthHorizontal, dist2Dimensions));
}

MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{

    savePoint = new QPushButton("save Point in File");
    savePoint->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    step = new QSpinBox;
    step->setRange(1, 50);
    step->setFixedWidth(75);
    QHBoxLayout* otherHBox = new QHBoxLayout;
    otherHBox->addWidget(step);
    otherHBox->addWidget(savePoint);

    //motor 1
    slider1 = new QSlider(Qt::Orientation::Horizontal);
    slider1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slider1->setMaximum(360);
    angle1 = new QSpinBox;
    angle1->setRange(0, 360);
    angle1->setFixedWidth(75);
    connect(slider1, &QSlider::valueChanged, angle1, &QSpinBox::setValue);
    connect(angle1, &QSpinBox::valueChanged, slider1, &QSlider::setValue);
    left1 = new PushButton(slider1, step, QIcon("://left.png"), "", this);
    right1 = new PushButton(slider1, step, QIcon("://right.png"), "", this);
    connect(left1, &QPushButton::clicked, left1, &PushButton::stepLeft);
    connect(right1, &QPushButton::clicked, left1, &PushButton::stepRight);
    QHBoxLayout* slider1HBox = new QHBoxLayout;
    slider1HBox->addWidget(slider1);
    slider1HBox->addWidget(angle1);
    slider1HBox->addWidget(left1);
    slider1HBox->addWidget(right1);


    QVBoxLayout* sliderVbox = new QVBoxLayout;
    sliderVbox->addLayout(slider1HBox);
    //sliderVbox->addLayout(slider2HBox);
    //sliderVbox->addLayout(slider3HBox);
    //sliderVbox->addLayout(slider4HBox);
    sliderVbox->addLayout(otherHBox);

    editor = new CodeEditor;
    QHBoxLayout* globalHbox = new QHBoxLayout;
    globalHbox->addLayout(sliderVbox);
    globalHbox->addWidget(editor);
    setLayout(globalHbox);
}
