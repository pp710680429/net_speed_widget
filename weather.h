#ifndef WEATHER_H
#define WEATHER_H

/*
 * 显示天气
 */

#include <QWidget>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QNetworkReply>
#include <QTimer>
#include "changcity.h"

class Weather : public QWidget
{
    Q_OBJECT
public:
    explicit Weather(QWidget *parent = 0);

private:
    QNetworkAccessManager * mNetwoekAccessManager;

    QString getCityId(QString cityName) ;
    QStringList getCityList() ;
    void getWeatherIcon(QString weather);

    QStringList mCityList;
    QString mCurrentCity;
    QString getCity();
    QMap<QString,QString> mCityToCityId;
    QString getAPIUrl(QString city);
    QTimer * timer;
    ChangCity * mChangeCity;

    QLabel * uWeatherIcon;
    QLabel * uWeatherText;
    QLabel * uWeatherTemperature;
    void InitUi();
signals:

private slots:
    void replyFinished(QNetworkReply*);

public slots:
    void reflashWeather();
    void setCurrentCity(QString city);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // WEATHER_H
