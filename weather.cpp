#include "weather.h"

#include <QFile>
#include <QDir>
#include <QSettings>
#include <QMenu>

Weather::Weather(QWidget *parent) : QWidget(parent)
{
    resize(80,64);  //高要和widget.h一样
    InitUi();

    QDir dir(QString(QDir::homePath() + "/.config/"));
    dir.mkdir("netSpeedWidget");
    QSettings setting(QString(QDir::homePath() + "/.config/netSpeedWidget/setting.ini"),QSettings::IniFormat);
    setting.beginGroup("weather");
    mCurrentCity = setting.value("CurrentCity").toString();
    qDebug() << mCurrentCity << "------------------";
    setting.endGroup();
    if(mCurrentCity.isEmpty())
        mCurrentCity = "遵义"; //如果没有设置当前城市，默认为遵义

    timer = new QTimer(this);
    mChangeCity = new ChangCity(this);
    timer->start(180000); //3小时更新一次

    mNetwoekAccessManager = new QNetworkAccessManager(this);

    connect(mNetwoekAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    connect(timer,&QTimer::timeout,this,[=](){
        mNetwoekAccessManager->get(QNetworkRequest(QUrl(getAPIUrl(getCity()))));
    });
    connect(mChangeCity,&ChangCity::setCurrentCity,this,&Weather::setCurrentCity);
    mNetwoekAccessManager->get(QNetworkRequest(QUrl(getAPIUrl(getCity()))));

}

QString  Weather::getCityId(QString cityName)
{

    if(mCityToCityId.isEmpty()){
        QFile loadFile(":/json/city");

        if(!loadFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "could't open projects json";
            return "错误"; //代表错误
        }
        QByteArray allData = loadFile.readAll();
        loadFile.close();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(
                    allData,&jsonError);

        if(!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)){
            if(doucment.isArray()){
                QJsonArray array = doucment.array();
                for(int i = 0; i < array.size() ; i++){
                    QJsonValue value1 = array.at(i);
                    if(value1.isObject()){
                        QJsonObject object = value1.toObject();
                        if(object.value("cityZh").isString())
                        {
                            if(object.value("id").isString()){
                                mCityToCityId[object.value("cityZh").toString()] = object.value("id").toString();
                            }
                            if(mCityList.isEmpty()){
                                mCityList.append(object.value("cityZh").toString());
                            }
                        }
                    }
                }
            }
        }
    }

    QString id;

    if(mCityToCityId[cityName].toDouble()){
        id = mCityToCityId[cityName];
        return id;
    }

    return "错误"; //代表错误
}

QStringList Weather::getCityList()
{
    this->getCityId("111");
    if(mCityList.isEmpty()){
        return QStringList() << "错误"; //代表错误
    }else {
        return mCityList;
    }
}

void Weather::getWeatherIcon(QString weather)
{
    QString path = "";
    if(weather == "xue"){
        path = ":/weather/image/xue.png";
    }else if(weather == "lei"){
        path = ":/weather/image/lei.png";
    }else if(weather == "shachen"){
        path = ":/weather/image/shachen.png";
    }else if(weather == "wu"){
        path = ":/weather/image/wu.png";
    }else if(weather == "bingbao"){
        path = ":/weather/image/bingbao.png";
    }else if(weather == "yun"){
        path = ":/weather/image/yun.png";
    }else if(weather == "yu"){
        path = ":/weather/image/yu.png";
    }else if(weather == "yin"){
        path = ":/weather/image/yin.png";
    }else if(weather == "qing"){
        path = ":/weather/image/qing.png";
    }else {
        path = ":/weather/image/error.png";
    }

    QPixmap img1(path);
    QSize lableSize(size().height()/2 , size().height()/2);
    uWeatherIcon->setFixedSize(lableSize);
    uWeatherIcon->setScaledContents(true);
    img1.scaled(uWeatherIcon->size(), Qt::KeepAspectRatio);
    uWeatherIcon->setPixmap(img1);
}

QString Weather::getCity()
{
    return mCurrentCity;
}

QString Weather::getAPIUrl(QString city)
{
    /*
     * API地址
     * https://www.tianqiapi.com/?action=v6
     */
    QString url = QString("https://www.tianqiapi.com/api/?version=v6&%1").arg(
                QString("cityid=%1").arg(getCityId(city)));

    return url;
}

void Weather::InitUi()
{
    uWeatherIcon = new QLabel(this);
    uWeatherText = new QLabel("加载中1",this);
    uWeatherTemperature = new QLabel("加载中2",this);
    QHBoxLayout * weartherlayout = new QHBoxLayout;
    weartherlayout->addWidget(uWeatherIcon);
    weartherlayout->addWidget(uWeatherText);
    QVBoxLayout * mainlayout = new QVBoxLayout(this);
    mainlayout->addLayout(weartherlayout);
    mainlayout->addWidget(uWeatherTemperature);

    weartherlayout->setMargin(0);
    mainlayout->setMargin(0);

    setStyleSheet("color:rgb(0, 0, 227)"); //为了和widget.h保持一致
}

void Weather::replyFinished(QNetworkReply *reply)
{
    QByteArray allData = reply->readAll();
    QJsonParseError jsonerror;
    QJsonDocument document = QJsonDocument::fromJson(allData,&jsonerror);

    if(!document.isEmpty() && (jsonerror.error == QJsonParseError::NoError))
    {
        if(document.isObject()){
            QJsonObject object = document.object();
            QString air_level =  object.value("air_level").toString(); //空气质量
            QString wea_img =  object.value("wea_img").toString(); //天气（获得的是图标）
            QString tem =  object.value("tem").toString(); //温度
            QString city =  object.value("city").toString(); //城市
            QString wea =  object.value("wea").toString(); //天气

            getWeatherIcon(wea_img); //内部已经自动设置图片
            uWeatherText->setText(QString(wea+" | "+air_level));
            uWeatherTemperature->setText(QString("%1℃  %2").arg(tem).arg(city));
        }
    }
    reply->deleteLater();
}

void Weather::reflashWeather()
{
    mNetwoekAccessManager->get(QNetworkRequest(QUrl(getAPIUrl(getCity()))));
}

void Weather::setCurrentCity(QString city)
{
    QSettings setting(QString(QDir::homePath() + "/.config/netSpeedWidget/setting.ini"),QSettings::IniFormat);
    setting.beginGroup("weather");
    setting.setValue("CurrentCity",city);
    qDebug() << setting.value("CurrentCity") << "++++++++++";
    setting.endGroup();

    mCurrentCity = city;
    //刷新，获得新城市的天气
    reflashWeather();
}

void Weather::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    static  QMenu *menu = new QMenu(this);
    if(menu->actions().length() == 0){
        menu->addAction(new QAction(tr("&刷新"), menu));
        menu->addAction(new QAction(tr("&更改城市"), menu));

        connect(menu,&QMenu::triggered,this,[=](QAction *action){
            if(action->text() == "&更改城市"){
                mChangeCity->setOldCity(mCurrentCity);
                mChangeCity->show();
            }else {
                this->reflashWeather();
            }
        });
    }
    menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
    menu->show();
}
