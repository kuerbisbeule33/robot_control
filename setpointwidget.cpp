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
#include "setpointwidget.h"

SetPointWidget::SetPointWidget(QWidget *parent)
    : QWidget{parent}
{
    QPushButton* savePoint = new QPushButton("save Point in File");
    savePoint->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QSpinBox* step = new QSpinBox;
    step->setRange(1, 50);
    step->setFixedWidth(75);
    QHBoxLayout* otherHBox = new QHBoxLayout;
    otherHBox->addWidget(step);
    otherHBox->addWidget(savePoint);

    //motor 1
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
    QHBoxLayout* slider1HBox = new QHBoxLayout;
    slider1HBox->addWidget(slider1);
    slider1HBox->addWidget(angle1);
    slider1HBox->addWidget(left1);
    slider1HBox->addWidget(right1);

    //motor 2
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
    QHBoxLayout* slider2HBox = new QHBoxLayout;
    slider2HBox->addWidget(slider2);
    slider2HBox->addWidget(angle2);
    slider2HBox->addWidget(left2);
    slider2HBox->addWidget(right2);

    //motor 3
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
    QHBoxLayout* slider3HBox = new QHBoxLayout;
    slider3HBox->addWidget(slider3);
    slider3HBox->addWidget(angle3);
    slider3HBox->addWidget(left3);
    slider3HBox->addWidget(right3);

    QVBoxLayout* slider123VBox = new QVBoxLayout;
    slider123VBox->addLayout(slider1HBox);
    slider123VBox->addLayout(slider2HBox);
    slider123VBox->addLayout(slider3HBox);
    slider123VBox->addLayout(otherHBox);

    QVBoxLayout* sliderVBox = new QVBoxLayout(this);
    sliderVBox->addLayout(slider123VBox);
    this->setLayout(slider123VBox);
}
