#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QSlider* slider, QSpinBox* step, QWidget *parent = nullptr);
    PushButton(QSlider* slider, QSpinBox* step,const QIcon &icon, const QString &text, QWidget *parent = nullptr);

private:
    QSlider* slider;
    QSpinBox* step;

public slots:
    void stepLeft();
    void stepRight();

};

#endif // PUSHBUTTON_H
