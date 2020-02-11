#include "api.h"

Api::Api(QObject *parent) : QObject(parent)
{
//    manager = new QNetworkAccessManager;
}

void Api::signIn(QString nameText, QString passText)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(signInFinished(QNetworkReply*)));
    qDebug() << "username:" << nameText << " password:" << passText;

    QString url = "/qt-product-check/sign-in-qt";

    QNetworkRequest *request = getRequestForUrl(url);

    QJsonObject obj;
    obj.insert("empNo", nameText);
    obj.insert("password", passText);
    QByteArray postData = QJsonDocument(obj).toJson();

    manager->post(*request,postData);
}

void Api::signInFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
//        QString string = QString::fromUtf8(bytes);
//        qDebug()<<bytes.toStdString().c_str();
//         ui->textBrowser->setText(string.toUtf8());

        QJsonObject obj = getJsonObjectFromByteArray(bytes);
        int code = obj.value("code").toInt();
        qDebug() << "code:" << code;

        if(code == 0){
            QJsonObject objData = obj.value("data").toObject();
            QString realName = objData.value("realName").toString();
            this->token = objData.value("token").toString();
            qDebug() << "realName:" << realName << " token:" << token;
            emit signInSignal(objData);
        }else{
            QString msg = obj.value("msg").toString();
            emit waring(msg);
        }
    }
    else
    {
        handleErrors(reply);
    }

    dispatch(reply);
}

//获得产品信息根据轮号
void Api::getProductMessage(QString productBarCode)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getProductMessageFinished(QNetworkReply*)));

    QString url = "/qt-product-check/get-product-message";
    QNetworkRequest *request = getRequestForUrl(url);

    QJsonObject obj;
    obj.insert("productBarCode", productBarCode);
    QByteArray postData = QJsonDocument(obj).toJson();

    manager->post(*request,postData);
}

void Api::getProductMessageFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
//        qDebug()<<bytes.toStdString().c_str();

        QJsonObject obj = getJsonObjectFromByteArray(bytes);
        int code = obj.value("code").toInt();
        qDebug() << "code:" << code;
        if(code == 0){
            QJsonObject objData = obj.value("data").toObject();
            emit getProductMessageSignal(objData);
        }else{
            QString msg = obj.value("msg").toString();
            emit waring(msg);
        }
    }
    else{
        handleErrors(reply);
    }

    dispatch(reply);
}

//释放
void Api::dispatch(QNetworkReply *reply)
{
    reply->manager()->deleteLater();
    reply->deleteLater();
}

//请求错误
void Api::handleErrors(QNetworkReply *reply)
{
    qDebug()<<"handle errors here";
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
    qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
//         qDebug(qPrintable(reply->errorString()));
}


/*
 *封装请求头
 */
QNetworkRequest* Api::getRequestForUrl(QString url)
{
    //POST
    //    request->setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    //    request->setRawHeader("Accept","text/html, application/xhtml+xml, */*");
    //    request->setRawHeader("Referer","http://localhost:8888/login");
    //    request->setRawHeader("Accept-Language","zh-CN");
    //    request->setRawHeader("X-Requested-With","XMLHttpRequest");
    //    request->setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
    //    request->setRawHeader("Content-Type","application/x-www-form-urlencoded");
    //    request->setRawHeader("Accept-Encoding","gzip,deflate");
    //    request->setRawHeader("Host","localhost:8888");
    //    request->setRawHeader("Content-Length","18");
    //    request->setRawHeader("Connection","Keep-Alive");
    //    request->setRawHeader("Cache-Control","no-cache");

    QNetworkRequest *request = new QNetworkRequest();
    request->setUrl(QUrl(this->serverUrl + url));
    request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request->setRawHeader("auth_key",this->auth_key.toUtf8());
//    if(this->token.size()>0){
//        qDebug() << "add token:" << token;
//        request->setRawHeader("token",this->token.toUtf8());
//    }
    return request;
}

/*
 *QByteArray 转为 QJsonObject
 */
QJsonObject Api::getJsonObjectFromByteArray(const QByteArray jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString);
    if( jsonDocument.isNull() ){
        qDebug()<< "===> QJsonDocument："<< jsonString;
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
