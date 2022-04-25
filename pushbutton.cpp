#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>
#include "pushbutton.h"

PushButton::PushButton(QSlider* slider, QSpinBox* step, QWidget *parent)
    : QPushButton{parent}, slider(slider), step(step)
{
}

PushButton::PushButton(QSlider* slider, QSpinBox* step, const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton{icon, text, parent}, slider(slider), step(step)
{
}

void PushButton::stepLeft(){
    slider->setValue(slider->value() - step->value());
}

void PushButton::stepRight(){
    slider->setValue(slider->value() + step->value());
}
