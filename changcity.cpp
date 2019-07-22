#include "changcity.h"
#include "ui_changcity.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

DWIDGET_USE_NAMESPACE

ChangCity::ChangCity(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangCity)
{
    ui->setupUi(this);

    setWindowTitle(tr("更改城市"));
    setFixedSize(size());

    InitData();
}

ChangCity::~ChangCity()
{
    delete ui;
}

void ChangCity::InitData()
{
    QFile loadFile(":/json/city");

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "could't open projects json";
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
                    mCity[object.value("cityZh").toString()] = object.value("cityZh").toString();
                }
            }
        }
    }
}



void ChangCity::on_pushButtonOk_clicked()
{
    if(!mCity[ui->lineEdit->text().trimmed()].isEmpty()){
        emit setCurrentCity(mCity[ui->lineEdit->text()]);
        QMessageBox::information(this,tr("提示"),QString("已经切换到：%1").arg(mCity[ui->lineEdit->text()]),QMessageBox::Ok);
        this->close();
    }else {
        QMessageBox::warning(this,tr("提示"),tr("没有找到这个城市,请确定是否输入正确!"),QMessageBox::Ok);
    }
}

void ChangCity::setOldCity(QString oldCity)
{
    ui->labelCurrentCity->setText(oldCity);
}
