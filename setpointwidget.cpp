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

SetPointWidget::SetPointWidget(QWidget *parent)
    : QWidget{parent}
{
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

    //motor 1
    QLabel* label1 = new QLabel("Horizontal");
    QSlider* slider1 = new QSlider(Qt::Orientation::Horizontal);
    slider1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slider1->setMaximum(360);
    QSpinBox* angle1 = new QSpinBox;
    angle1->setRange(0, 360);
    angle1->setFixedWidth(75);
    connect(slider1, &QSlider::valueChanged, angle1, &QSpinBox::setValue);
    connect(angle1, &QSpinBox::valueChanged, slider1, &QSlider::setValue);
    PushButton* left1 = new PushButton(slider1, step, QIcon("://left.png"), "");
    PushButton* right1 = new PushButton(slider1, step, QIcon("://right.png"), "");
    connect(left1, &QPushButton::clicked, left1, &PushButton::stepLeft);
    connect(right1, &QPushButton::clicked, right1, &PushButton::stepRight);

    //motor 2
    QLabel* label2 = new QLabel("Vertical  ");
    QSlider* slider2 = new QSlider(Qt::Orientation::Horizontal);
    slider2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slider2->setMaximum(360);
    QSpinBox* angle2 = new QSpinBox;
    angle2->setRange(0, 360);
    angle2->setFixedWidth(75);
    connect(slider2, &QSlider::valueChanged, angle2, &QSpinBox::setValue);
    connect(angle2, &QSpinBox::valueChanged, slider2, &QSlider::setValue);
    PushButton* left2 = new PushButton(slider2, step, QIcon("://left.png"), "");
    PushButton* right2 = new PushButton(slider2, step, QIcon("://right.png"), "");
    connect(left2, &QPushButton::clicked, left2, &PushButton::stepLeft);
    connect(right2, &QPushButton::clicked, right2, &PushButton::stepRight);

    //motor 3
    QLabel* label3 = new QLabel("Rotation  ");
    QSlider* slider3 = new QSlider(Qt::Orientation::Horizontal);
    slider3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slider3->setMaximum(360);
    QSpinBox* angle3 = new QSpinBox;
    angle3->setRange(0, 360);
    angle3->setFixedWidth(75);
    connect(slider3, &QSlider::valueChanged, angle3, &QSpinBox::setValue);
    connect(angle3, &QSpinBox::valueChanged, slider3, &QSlider::setValue);
    PushButton* left3 = new PushButton(slider3, step, QIcon("://left.png"), "");
    PushButton* right3 = new PushButton(slider3, step, QIcon("://right.png"), "");
    connect(left3, &QPushButton::clicked, left3, &PushButton::stepLeft);
    connect(right3, &QPushButton::clicked, right3, &PushButton::stepRight);

    //xyz Layout
    QGridLayout* xyzBox = new QGridLayout;
    xyzBox->addWidget(label1, 0, 0);
    xyzBox->addWidget(angle1, 0, 1);
    xyzBox->addWidget(slider1, 0, 2);
    xyzBox->addWidget(left1, 0, 3);
    xyzBox->addWidget(right1, 0, 4);
    xyzBox->addWidget(label2, 1, 0);
    xyzBox->addWidget(angle2, 1, 1);
    xyzBox->addWidget(slider2, 1, 2);
    xyzBox->addWidget(left2, 1, 3);
    xyzBox->addWidget(right2, 1, 4);
    xyzBox->addWidget(label3, 2, 0);
    xyzBox->addWidget(angle3, 2, 1);
    xyzBox->addWidget(slider3, 2, 2);
    xyzBox->addWidget(left3, 2, 3);
    xyzBox->addWidget(right3, 2, 4);

    //greifer optionen
    QLabel* stepGripperLabel = new QLabel("Step-Width:");
    QSpinBox* stepGripper = new QSpinBox;
    stepGripper->setRange(1, 50);
    stepGripper->setFixedWidth(75);
    savePointGripper = new QPushButton("save Point in File");
    savePointGripper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(savePointGripper, &QPushButton::clicked, this, &SetPointWidget::newGripperGcodeEmitted);
    //greifer slider layout
    QHBoxLayout* gripperSetHBox = new QHBoxLayout;
    gripperSetHBox->addWidget(savePointGripper);
    gripperSetHBox->addWidget(stepGripperLabel);
    gripperSetHBox->addWidget(stepGripper);

    //greifer slider
    labelGripper = new QLabel("G01");
    sliderGripper = new QSlider(Qt::Orientation::Horizontal);
    sliderGripper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sliderGripper->setMaximum(90);
    QSpinBox* angleGripper = new QSpinBox;
    angleGripper->setRange(0, 90);
    connect(sliderGripper, &QSlider::valueChanged, angleGripper, &QSpinBox::setValue);
    connect(angleGripper, &QSpinBox::valueChanged, sliderGripper, &QSlider::setValue);
    PushButton* leftGripper = new PushButton(sliderGripper, stepGripper, QIcon("://left.png"), "");
    PushButton* rightGripper = new PushButton(sliderGripper, stepGripper, QIcon("://right.png"), "");
    connect(leftGripper, &QPushButton::clicked, leftGripper, &PushButton::stepLeft);
    connect(rightGripper, &QPushButton::clicked, rightGripper, &PushButton::stepRight);
    //greifer slider layout
    QHBoxLayout* gripperPosHBox = new QHBoxLayout;
    gripperPosHBox->addWidget(labelGripper);
    gripperPosHBox->addWidget(angleGripper);
    gripperPosHBox->addWidget(sliderGripper);
    gripperPosHBox->addWidget(leftGripper);
    gripperPosHBox->addWidget(rightGripper);

    //globales Layout
    QVBoxLayout* setPointVBox = new QVBoxLayout(this);
    setPointVBox->addLayout(xyzBox);
    setPointVBox->addLayout(pointBox);
    QSpacerItem* spacer1 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    setPointVBox->addSpacerItem(spacer1);
    setPointVBox->addLayout(gripperPosHBox);
    setPointVBox->addLayout(gripperSetHBox);
    QSpacerItem* spacer2 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    setPointVBox->addSpacerItem(spacer2);
    log = new QTextBrowser;
    setPointVBox->addWidget(log);
    this->setLayout(setPointVBox);
}

//slots
void SetPointWidget::newPosGcodeEmitted(){
    QString code = this->gCode->currentText() + " x" + xPos->cleanText() + " y" + yPos->cleanText() + " z" + zPos->cleanText();
    emit newPosGcode(code);
}

void SetPointWidget::newGripperGcodeEmitted(){
    QString code = this->labelGripper->text() + " a" + QString().setNum(this->sliderGripper->value());
    emit newGripperGcode(code);
}
