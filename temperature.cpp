#include "temperature.h"

#include <QFile>
#include <QColor>
#include <QRgb>

DWIDGET_USE_NAMESPACE

Temperature::Temperature(QWidget *parent) : QWidget(parent)
{
    Init();
}

void Temperature::InitUi()
{
    mCircleProgress = new DCircleProgress(this);

    QVBoxLayout * mainlayout = new QVBoxLayout(this);
    mainlayout->addWidget(mCircleProgress);

    mCircleProgress->setFixedSize(this->size().height(),this->size().height());
    mainlayout->setMargin(0);
}

void Temperature::Init()
{
    InitUi();

    QColor b;
    b.setRgb(248, 248, 248);
    mCircleProgress->setBackgroundColor(b); //和widget.ui中的widget的背景颜色一样
    reflashTemperature();
    timer = new QTimer;
    timer->start(2000); //每两秒扫一次
    connect(timer,&QTimer::timeout,this,[=](){
        reflashTemperature();
    });
}

unsigned int Temperature::getCpuTemperature()
{
    //deepin的CPU温度是存放在这儿的
    QFile file("/sys/class/thermal/thermal_zone0/temp");
    if(!file.open(QIODevice::ReadOnly)){
        return 0;
    }

    QString tem = QString(file.readAll());

    unsigned int i = (unsigned int) tem.toInt() ;
    return (i / 1000);
    file.close();
}

void Temperature::reflashTemperature()
{
    QColor color ;
    unsigned int i = getCpuTemperature();
    if(i >= 75){
        color = Qt::red;
        mCircleProgress->setChunkColor(color);
    }else {
        color = Qt::green;
        mCircleProgress->setChunkColor(color);
    }

    QString Tcontext = /*"<font style='font-size:12px;'>" + */QString::number(i) + "℃";
    mCircleProgress->setText(Tcontext);
    mCircleProgress->setValue(getCpuTemperature());
}
