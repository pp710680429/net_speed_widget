#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    Init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Init()
{
    isPress = false;
    QPixmap img1(":/icon/上传");
    QPixmap img2(":/icon/下载");
    QSize lableSize(size().height()/2 , size().height()/2);
    ui->label->setFixedSize(lableSize);
    ui->label_2->setFixedSize(lableSize);
    ui->label->setScaledContents(true);
    ui->label_2->setScaledContents(true);
    img1.scaled(ui->label->size(), Qt::KeepAspectRatio);
    img2.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
    ui->label->setPixmap(img1);
    ui->label_2->setPixmap(img2);

    setAttribute(Qt::WA_TranslucentBackground); //设置背景透明

    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    this->move(pDesktopWidget->width() - this->size().width() - 200,pDesktopWidget->height() / 2 + 100);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint
                     | Qt::Tool | Qt::FramelessWindowHint);

    timer = new QTimer;
    weather = new Weather;
    weather->resize(80,size().height());
    timer->start(1500);
    ui->horizontalLayout_2->addWidget(weather);

    getNetworkBandWidth(olddown,oldup);

    connect(timer,&QTimer::timeout,this,[=](){
        getNetworkBandWidth(down,up);

        ui->label_up->setText(this->getFormatSpeed(up - oldup));
        ui->label_down->setText(this->getFormatSpeed(down - olddown));

        olddown = down;
        oldup = up;
    });
}

void Widget::getNetworkBandWidth(unsigned long long &receiveBytes, unsigned long long &sendBytes)
{
    char *buf;
    static int bufsize;
    FILE *devfd;

    buf = (char *) calloc(255, 1);
    bufsize = 255;
    devfd = fopen("/proc/net/dev", "r");

    // Ignore the first two lines of the file.
    fgets(buf, bufsize, devfd);
    fgets(buf, bufsize, devfd);

    receiveBytes = 0;
    sendBytes = 0;

    while (fgets(buf, bufsize, devfd)) {
        unsigned long long int rBytes, sBytes;
        char *line = strdup(buf);

        char *dev;
        dev = strtok(line, ":");

        // Filter lo (virtual network device).
        if (QString::fromStdString(dev).trimmed() != "lo") {
            sscanf(buf + strlen(dev) + 2, "%llu %*d %*d %*d %*d %*d %*d %*d %llu", &rBytes, &sBytes);

            receiveBytes += rBytes;
            sendBytes += sBytes;
        }

        free(line);
    }

    fclose(devfd);
    free(buf);
}

QString Widget::getFormatSpeed( unsigned long long i)
{
    i = i / 1024.0;
    if(i < 1024.0)
        return QString::number(i, 'f', 2) + " K/s";
    else if(i < (1024.0 * 1024.0)) {
        return QString::number(i / 1024.0 , 'f', 2) + " M/s";
    }else {
        return QString::number(i / 1024.0 * 1024.0 , 'f', 2) + " G/s";
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    static QPoint p;
    if(isPress){
        //qDebug() << event->pos() << "     " << event->globalPos();
        p = event->globalPos() - oldPoint;
        //qDebug() << p;
        move(p);
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    isPress = true;
    oldPoint = event->pos();
    Q_UNUSED(event)
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    isPress = false;
    Q_UNUSED(event)
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    static  QMenu *menu = new QMenu(this);
    if(menu->actions().length() == 0){
        menu->addAction(new QAction(tr("&刷新"), menu));
        menu->addAction(new QAction(tr("&退出"), menu));

        connect(menu,&QMenu::triggered,this,[=](QAction *action){
            if(action->text() == "&退出"){
                this->close();
                QApplication::quit();
            }else {
                weather->reflashWeather();
            }
        });
    }
    menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
    menu->show();
}

