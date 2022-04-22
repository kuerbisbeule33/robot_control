#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{
    slider1 = new QSlider(Qt::Orientation::Horizontal);
    slider1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    angle1 = new QSpinBox;
    left1 = new QPushButton;
    right1 = new QPushButton;
    slider1HBox = new QHBoxLayout;
    slider1HBox->addWidget(slider1);
    slider1HBox->addWidget(angle1);
    slider1HBox->addWidget(left1);
    slider1HBox->addWidget(right1);

    slider2 = new QSlider(Qt::Orientation::Horizontal);
    slider2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    angle2 = new QSpinBox;
    left2 = new QPushButton;
    right2 = new QPushButton;
    slider2HBox = new QHBoxLayout;
    slider2HBox->addWidget(slider2);
    slider2HBox->addWidget(angle2);
    slider2HBox->addWidget(left2);
    slider2HBox->addWidget(right2);

    slider3 = new QSlider(Qt::Orientation::Horizontal);
    slider3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    angle3 = new QSpinBox;
    left3 = new QPushButton;
    right3 = new QPushButton;
    slider3HBox = new QHBoxLayout;
    slider3HBox->addWidget(slider3);
    slider3HBox->addWidget(angle3);
    slider3HBox->addWidget(left3);
    slider3HBox->addWidget(right3);

    slider4 = new QSlider(Qt::Orientation::Horizontal);
    slider4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    angle4 = new QSpinBox;
    left4 = new QPushButton;
    right4 = new QPushButton;
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
