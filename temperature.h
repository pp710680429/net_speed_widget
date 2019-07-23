#ifndef TEMPERATURE_H
#define TEMPERATURE_H

/*
 * 显示CPU温度
 */

#include <dcircleprogress.h>
#include <QVBoxLayout>
#include <QDebug>
#include <QWidget>
#include <QTimer>

DWIDGET_USE_NAMESPACE

class Temperature : public QWidget
{
    Q_OBJECT
public:
    explicit Temperature(QWidget *parent = 0);

private:
    DCircleProgress * mCircleProgress;

    void InitUi();
    void Init();

    unsigned int getCpuTemperature();
    QTimer * timer;
    void reflashTemperature();
signals:

public slots:
};

#endif // TEMPERATURE_H
