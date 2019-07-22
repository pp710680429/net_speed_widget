#ifndef CHANGCITY_H
#define CHANGCITY_H

#include <ddialog.h>
#include <QDebug>

DWIDGET_USE_NAMESPACE

namespace Ui {
    class ChangCity;
}

class ChangCity : public QDialog
{
    Q_OBJECT

public:
    explicit ChangCity(QWidget *parent = 0);
    ~ChangCity();

private:
    void InitData();
    QMap<QString , QString> mCity;

private slots:
    void on_pushButtonOk_clicked();
public:
    void setOldCity(QString oldCity);
signals:
    void setCurrentCity(QString city);

private:
    Ui::ChangCity *ui;
};

#endif // CHANGCITY_H
