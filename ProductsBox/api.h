#ifndef API_H
#define API_H

#include <QObject>
#include <QDebug>
#include <QtNetwork>

class Api : public QObject
{
    Q_OBJECT


public:
    explicit Api(QObject *parent = nullptr);

    void signIn(QString nameText,QString passText);
    void getProductMessage(QString productBarCode);

private:
//    QNetworkAccessManager *manager;

    QString const serverUrl = "http://www.lvsiqi.com:8080/charger-mes";
    QString const auth_key = "c592e5258fdb11e99e34a44cc8015d9f";
    QString token;


    QJsonObject getJsonObjectFromByteArray(const QByteArray jsonString);
    QNetworkRequest* getRequestForUrl(QString);
    void dispatch(QNetworkReply *reply);
    void handleErrors(QNetworkReply *reply);

public slots:
    void signInFinished(QNetworkReply *reply);
    void getProductMessageFinished(QNetworkReply *reply);

signals:
    void waring(QString);
    void signInSignal(QJsonObject);
    void getProductMessageSignal(QJsonObject);
};

#endif // API_H
