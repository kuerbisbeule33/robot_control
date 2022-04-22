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

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    QHBoxLayout* globalHbox;
    QVBoxLayout* sliderVbox;
    QHBoxLayout* slider1HBox;
    QHBoxLayout* slider2HBox;
    QHBoxLayout* slider3HBox;
    QHBoxLayout* slider4HBox;
    QHBoxLayout* otherHBox;
    QSpinBox* angle1;
    QSpinBox* angle2;
    QSpinBox* angle3;
    QSpinBox* angle4;
    QPushButton* left1;
    QPushButton* right1;
    QPushButton* left2;
    QPushButton* right2;
    QPushButton* left3;
    QPushButton* right3;
    QPushButton* left4;
    QPushButton* right4;
    QSpinBox* step;
    QPushButton* savePoint;
    QSlider* slider1;
    QSlider* slider2;
    QSlider* slider3;
    QSlider* slider4;
    CodeEditor* editor;

signals:


};

#endif // MAINWIDGET_H
