#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QNetworkProxy>
#include <QSslError>

#include "ApiRequest.h"

ApiRequest::ApiRequest(QObject *parent) : QObject(parent)
{
    _transport = new QNetworkAccessManager;


    QNetworkProxy proxy(QNetworkProxy::HttpProxy, "192.168.1.48", 8081);

    _transport->setProxy(proxy);

    connect(_transport, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(replySSLErrors(QNetworkReply*,QList<QSslError>)));

}

void ApiRequest::responseReady()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray data = reply->readAll();
    QJsonDocument jDoc = QJsonDocument::fromJson(data);
    QJsonObject jObj = jDoc.object();
    QString strFromObj = QJsonDocument(jObj).toJson(QJsonDocument::Compact).toStdString().c_str();
    if (strFromObj.contains("result")) {
        QJsonValue jVal = jObj.value("result");
//        qDebug() << "Reply: " << strFromObj;
        emit dataReady(jVal);
    } else {
        qDebug() << "Error in API request!";
        qDebug() << "+++++++++++++++++++";
        qDebug() << data;
    }
}

void
ApiRequest::thirdPartyResponseReady()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    emit thirdPartyDataReady(reply);
}

void
ApiRequest::thirdPartyGetRequest(QNetworkRequest& request, bool includeAuthorization)
{
    QString accessToken = _settings.value("accessToken").toString();

    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("X-Requested-With", "XMLHttpRequest");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Yandex-Music-API");
    request.setRawHeader("X-Yandex-Music-Client", "YandexMusicAndroid/23020251");

    if (accessToken.size() > 0 && includeAuthorization) {
        request.setRawHeader("Authorization", QString("OAuth %1").arg(accessToken).toLatin1());
    }

    QNetworkReply* reply = _transport->get(request);

    connect(reply, &QNetworkReply::finished, this, &ApiRequest::thirdPartyResponseReady, Qt::UniqueConnection);
}

void
ApiRequest::thirdPartyPostRequest(QNetworkRequest& request, QByteArray& data, bool includeAuthorization)
{
    QString accessToken = _settings.value("accessToken").toString();

    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("X-Requested-With", "XMLHttpRequest");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Yandex-Music-API");
    request.setRawHeader("X-Yandex-Music-Client", "YandexMusicAndroid/23020251");

    if (accessToken.size() > 0 && includeAuthorization) {
        request.setRawHeader("Authorization", QString("OAuth %1").arg(accessToken).toLatin1());
    }


    QNetworkReply* reply = _transport->post(request, data);

    connect(reply, &QNetworkReply::finished, this, &ApiRequest::thirdPartyResponseReady, Qt::UniqueConnection);
}

void
ApiRequest::apiGetRequest(QNetworkRequest& request)
{
    QString accessToken = _settings.value("accessToken").toString();

    if (accessToken.size() > 0)
        request.setRawHeader("Authorization", "OAuth " + accessToken.toUtf8());

    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("X-Yandex-Music-Content-Type", "adult");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
    request.setRawHeader("X-Yandex-Music-Client", "YandexMusic/519");
    request.setRawHeader("X-Yandex-Music-Device", "os=iOS; os_version=12.2; manufacturer=Apple; model=iPhone8,4; clid=0; device_id=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA; uuid=70db875aae45466dbe932244c10a62c1");

    QNetworkReply* reply = _transport->get(request);

    connect(reply, &QNetworkReply::finished, this, &ApiRequest::responseReady, Qt::UniqueConnection);
}


void
ApiRequest::apiPostRequest(QNetworkRequest& request, QByteArray& data)
{
    QString accessToken = _settings.value("accessToken").toString();

    request.setRawHeader("Authorization", "OAuth " + accessToken.toUtf8());
    if (data.contains("{")) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    } else {
        request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
    }
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("X-Yandex-Music-Content-Type", "adult");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
    request.setRawHeader("X-Yandex-Music-Client", "YandexMusic/519");
    request.setRawHeader("X-Yandex-Music-Device", "os=iOS; os_version=12.2; manufacturer=Apple; model=iPhone8,4; clid=0; device_id=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA; uuid=70db875aae45466dbe932244c10a62c1");

    QNetworkReply* reply = _transport->post(request, data);

    connect(reply, &QNetworkReply::finished, this, &ApiRequest::responseReady, Qt::UniqueConnection);
}

void ApiRequest::replySSLErrors(QNetworkReply *reply, QList<QSslError> errors)
{
    QList<QSslError> ignoreErrors;
    foreach(QSslError error, errors)
    {

        switch( error.error() )
        {
        case QSslError::CertificateUntrusted:
        case QSslError::SelfSignedCertificate:
        case QSslError::SelfSignedCertificateInChain:
            ignoreErrors << error;
            break;
        default:
            ignoreErrors << error;
            break;
        }
    }
    reply->ignoreSslErrors(ignoreErrors);

}
