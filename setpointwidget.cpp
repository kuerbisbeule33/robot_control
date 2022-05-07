#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <codeeditor.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <setpointwidget.h>
#include <QGridLayout>
#include <QLine>
#include <QSpacerItem>
#include <QTextBrowser>
#include "pushbutton.h"
#include "setpointwidget.h"
#include "kinematics.h"

SetPointWidget::SetPointWidget(QWidget *parent)
    : QWidget{parent}
{
    //greifer optionen //motor 1
    QLabel* stepGripperLabel1 = new QLabel("Step-Width:");
    QSpinBox* stepGripper1 = new QSpinBox;
    stepGripper1->setRange(1, 50);
    stepGripper1->setFixedWidth(75);
    savePointGripper1 = new QPushButton("save Point in File");
    savePointGripper1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(savePointGripper1, &QPushButton::clicked, this, &SetPointWidget::newGripperGcodeEmitted);
    //greifer slider layout
    QHBoxLayout* gripperSetHBox1 = new QHBoxLayout;
    gripperSetHBox1->addWidget(savePointGripper1);
    gripperSetHBox1->addWidget(stepGripperLabel1);
    gripperSetHBox1->addWidget(stepGripper1);

    //greifer slider // motor 1
    labelGripper1 = new QLabel("G01");
    sliderGripper1 = new QSlider(Qt::Orientation::Horizontal);
    sliderGripper1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sliderGripper1->setMaximum(90);
    QSpinBox* angleGripper1 = new QSpinBox;
    angleGripper1->setRange(0, 90);
    connect(sliderGripper1, &QSlider::valueChanged, angleGripper1, &QSpinBox::setValue);
    connect(angleGripper1, &QSpinBox::valueChanged, sliderGripper1, &QSlider::setValue);
    PushButton* leftGripper1 = new PushButton(sliderGripper1, stepGripper1, QIcon("://left.png"), "");
    PushButton* rightGripper1 = new PushButton(sliderGripper1, stepGripper1, QIcon("://right.png"), "");
    connect(leftGripper1, &QPushButton::clicked, leftGripper1, &PushButton::stepLeft);
    connect(rightGripper1, &QPushButton::clicked, rightGripper1, &PushButton::stepRight);
    //greifer slider layout
    QHBoxLayout* gripperPosHBox1 = new QHBoxLayout;
    gripperPosHBox1->addWidget(labelGripper1);
    gripperPosHBox1->addWidget(angleGripper1);
    gripperPosHBox1->addWidget(sliderGripper1);
    gripperPosHBox1->addWidget(leftGripper1);
    gripperPosHBox1->addWidget(rightGripper1);

    //pos settings
    gCode = new QComboBox();
    gCode->addItem("G00");
    QLabel* xPosLabel = new QLabel("X-Pos");
    QLabel* yPosLabel = new QLabel("Y-Pos");
    QLabel* zPosLabel = new QLabel("Z-Pos");
    QLabel* gCodeLabel = new QLabel("G-Code-Command");
    QLabel* stepLabel = new QLabel("Step-Width");
    xPos = new QSpinBox();
    yPos = new QSpinBox();
    zPos = new QSpinBox();
    savePoint = new QPushButton("save Point in File");
    savePoint->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(savePoint, &QPushButton::clicked, this, &SetPointWidget::newPosGcodeEmitted);
    QSpinBox* step = new QSpinBox;
    step->setRange(1, 50);
    step->setFixedWidth(75);
    QGridLayout* pointBox = new QGridLayout;
    pointBox->addWidget(gCodeLabel, 0, 0);
    pointBox->addWidget(xPosLabel, 0, 1);
    pointBox->addWidget(yPosLabel, 0, 2);
    pointBox->addWidget(zPosLabel, 0, 3);
    pointBox->addWidget(stepLabel, 0, 5);
    pointBox->addWidget(gCode, 1, 0);
    pointBox->addWidget(xPos, 1, 1);
    pointBox->addWidget(yPos, 1, 2);
    pointBox->addWidget(zPos, 1, 3);
    pointBox->addWidget(savePoint, 1, 4);
    pointBox->addWidget(step, 1, 5);

    xPos->setRange(50,500);
    yPos->setRange(50,500);
    zPos->setRange(50,500);

    //vertical //alt: motor 2 //neu:motor 2
    QLabel* labelVertical2 = new QLabel("Vertical  ");
    sliderVertical2 = new QSlider(Qt::Orientation::Horizontal);
    sliderVertical2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sliderVertical2->setRange(0,180);
    QSpinBox* angleVertical2 = new QSpinBox;
    angleVertical2->setRange(0,180);
    angleVertical2->setFixedWidth(75);
    connect(sliderVertical2, &QSlider::valueChanged, angleVertical2, &QSpinBox::setValue);
    connect(angleVertical2, &QSpinBox::valueChanged, sliderVertical2, &QSlider::setValue);
    PushButton* leftVertical2 = new PushButton(sliderVertical2, step, QIcon("://left.png"), "");
    PushButton* rightVertical2 = new PushButton(sliderVertical2, step, QIcon("://right.png"), "");
    connect(leftVertical2, &QPushButton::clicked, leftVertical2, &PushButton::stepLeft);
    connect(rightVertical2, &QPushButton::clicked, rightVertical2, &PushButton::stepRight);

    //horizontal //alt: motor 1 //neu: motor 3
    QLabel* labelHorizontal3 = new QLabel("Horizontal");
    sliderHorizontal3 = new QSlider(Qt::Orientation::Horizontal);
    sliderHorizontal3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sliderHorizontal3->setRange(0,180);
    QSpinBox* angleHorizontal3 = new QSpinBox;
    angleHorizontal3->setRange(0,180);
    angleHorizontal3->setFixedWidth(75);
    connect(sliderHorizontal3, &QSlider::valueChanged, angleHorizontal3, &QSpinBox::setValue);
    connect(angleHorizontal3, &QSpinBox::valueChanged, sliderHorizontal3, &QSlider::setValue);
    PushButton* leftHorizontal3 = new PushButton(sliderHorizontal3, step, QIcon("://left.png"), "");
    PushButton* rightHorizontal3 = new PushButton(sliderHorizontal3, step, QIcon("://right.png"), "");
    connect(leftHorizontal3, &QPushButton::clicked, leftHorizontal3, &PushButton::stepLeft);
    connect(rightHorizontal3, &QPushButton::clicked, rightHorizontal3, &PushButton::stepRight);

    //rotation //alt: motor 3 // neu: motor 4
    QLabel* labelRotation4 = new QLabel("Rotation  ");
    sliderRotation4 = new QSlider(Qt::Orientation::Horizontal);
    sliderRotation4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sliderRotation4->setRange(0,180);
    QSpinBox* angleRotation4 = new QSpinBox;
    angleRotation4->setRange(0,180);
    angleRotation4->setFixedWidth(75);
    connect(sliderRotation4, &QSlider::valueChanged, angleRotation4, &QSpinBox::setValue);
    connect(angleRotation4, &QSpinBox::valueChanged, sliderRotation4, &QSlider::setValue);
    PushButton* leftRotation4 = new PushButton(sliderRotation4, step, QIcon("://left.png"), "");
    PushButton* rightRotation4 = new PushButton(sliderRotation4, step, QIcon("://right.png"), "");
    connect(leftRotation4, &QPushButton::clicked, leftRotation4, &PushButton::stepLeft);
    connect(rightRotation4, &QPushButton::clicked, rightRotation4, &PushButton::stepRight);

    //xyz Layout
    QGridLayout* xyzBox = new QGridLayout;
    xyzBox->addWidget(labelVertical2, 0, 0);
    xyzBox->addWidget(angleVertical2, 0, 1);
    xyzBox->addWidget(sliderVertical2, 0, 2);
    xyzBox->addWidget(leftVertical2, 0, 3);
    xyzBox->addWidget(rightVertical2, 0, 4);
    xyzBox->addWidget(labelHorizontal3, 1, 0);
    xyzBox->addWidget(angleHorizontal3, 1, 1);
    xyzBox->addWidget(sliderHorizontal3, 1, 2);
    xyzBox->addWidget(leftHorizontal3, 1, 3);
    xyzBox->addWidget(rightHorizontal3, 1, 4);
    xyzBox->addWidget(labelRotation4, 2, 0);
    xyzBox->addWidget(angleRotation4, 2, 1);
    xyzBox->addWidget(sliderRotation4, 2, 2);
    xyzBox->addWidget(leftRotation4, 2, 3);
    xyzBox->addWidget(rightRotation4, 2, 4);



    //globales Layout
    QVBoxLayout* setPointVBox = new QVBoxLayout(this);
    setPointVBox->addLayout(gripperPosHBox1);
    setPointVBox->addLayout(gripperSetHBox1);
    QSpacerItem* spacer1 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    setPointVBox->addSpacerItem(spacer1);
    setPointVBox->addLayout(xyzBox);
    setPointVBox->addLayout(pointBox);
    QSpacerItem* spacer2 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    setPointVBox->addSpacerItem(spacer2);
    log = new QTextBrowser;
    setPointVBox->addWidget(log);
    this->setLayout(setPointVBox);

    //connects
    connect(sliderVertical2, &QSlider::valueChanged, this, &SetPointWidget::changeXYZpos);
    connect(sliderHorizontal3, &QSlider::valueChanged, this, &SetPointWidget::changeXYZpos);
    connect(sliderRotation4, &QSlider::valueChanged, this, &SetPointWidget::changeXYZpos);
    connect(xPos, &QSpinBox::valueChanged, this, &SetPointWidget::changeAnglePos);
    connect(yPos, &QSpinBox::valueChanged, this, &SetPointWidget::changeAnglePos);
    connect(zPos, &QSpinBox::valueChanged, this, &SetPointWidget::changeAnglePos);
}

//slots
void SetPointWidget::newPosGcodeEmitted(){
    QString code = this->gCode->currentText() + " x" + xPos->cleanText() + " y" + yPos->cleanText() + " z" + zPos->cleanText();
    emit newPosGcode(code);
}

void SetPointWidget::newGripperGcodeEmitted(){
    QString code = this->labelGripper1->text() + " d" + QString().setNum(this->sliderGripper1->value());
    emit newGripperGcode(code);
}

void SetPointWidget::changeXYZpos(){
    disconnect(this->xPos, &QSpinBox::valueChanged, this, &SetPointWidget::changeAnglePos);
    disconnect(this->yPos, &QSpinBox::valueChanged, this, &SetPointWidget::changeAnglePos);
    disconnect(this->zPos, &QSpinBox::valueChanged, this, &SetPointWidget::changeAnglePos);
    Angles angles;
    angles.q1 =this->sliderRotation4->value();
    angles.q2 =this->sliderVertical2->value();
    angles.q3 = this->sliderHorizontal3->value();
    Point point = forwardKinematics(angles);
    xPos->setValue(point.x);
    yPos->setValue(point.y);
    zPos->setValue(point.z);
    connect(this->xPos, &QSpinBox::valueChanged, this, &SetPointWidget::changeAnglePos);
    connect(this->yPos, &QSpinBox::valueChanged, this, &SetPointWidget::changeAnglePos);
    connect(this->zPos, &QSpinBox::valueChanged, this, &SetPointWidget::changeAnglePos);
}

void SetPointWidget::changeAnglePos(){
    disconnect(this->sliderVertical2, &QSlider::valueChanged, this, &SetPointWidget::changeXYZpos);
    disconnect(this->sliderHorizontal3, &QSlider::valueChanged, this, &SetPointWidget::changeXYZpos);
    disconnect(this->sliderRotation4, &QSlider::valueChanged, this, &SetPointWidget::changeXYZpos);
    Point point {static_cast<quint16>(this->xPos->value()), static_cast<quint16>(this->yPos->value()), static_cast<quint16>(this->zPos->value())};
    Angles angles = reverseKinematics(point);
    if(!checkErrorJointLimits(angles)){
        angles = mapKinematicsToServoAngles(angles);
        this->sliderVertical2->setValue(angles.q2);
        this->sliderHorizontal3->setValue(angles.q3);
        this->sliderRotation4->setValue(angles.q1);
    }
    connect(this->sliderVertical2, &QSlider::valueChanged, this, &SetPointWidget::changeXYZpos);
    connect(this->sliderHorizontal3, &QSlider::valueChanged, this, &SetPointWidget::changeXYZpos);
    connect(this->sliderRotation4, &QSlider::valueChanged, this, &SetPointWidget::changeXYZpos);
}
