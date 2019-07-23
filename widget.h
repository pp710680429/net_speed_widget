#ifndef WIDGET_H
#define WIDGET_H

#include <QMoveEvent>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include "weather.h"
#include "temperature.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    void Init();

    bool isPress;
    QPoint oldPoint;
    QTimer * timer;
    Weather * weather;
    Temperature * temperature;

    void getNetworkBandWidth(unsigned long long int &receiveBytes, unsigned long long int &sendBytes);
    QString getFormatSpeed(unsigned long long i);
    unsigned long long  oldup,olddown,up,down;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // WIDGET_H
