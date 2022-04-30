#ifndef SETPOINTWIDGET_H
#define SETPOINTWIDGET_H

#include <QWidget>
#include <QTextBrowser>

class SetPointWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SetPointWidget(QWidget *parent = nullptr);

public:
    QTextBrowser* log;
signals:

};

#endif // SETPOINTWIDGET_H
