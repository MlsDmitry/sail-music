#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkProxy>
#include <QSslError>

#include "YaClient.h"
#include "settings.h"
#include "Core.h"

YaClient::YaClient(QObject *parent) : QObject(parent)
{
    _transport = new QNetworkAccessManager();


    QNetworkProxy proxy(QNetworkProxy::HttpProxy, "192.168.1.48", 8081);

    _transport->setProxy(proxy);

    connect(_transport, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(replySSLErrors(QNetworkReply*,QList<QSslError>)));

}


void YaClient::replySSLErrors(QNetworkReply *reply, QList<QSslError> errors)
{
    QList<QSslError> ignoreErrors;
    foreach(QSslError error, errors)
    {
//        qDebug() << QLatin1String("Ignoring SSL error: ")
//                   << (int)error.error()
//                   << QLatin1String(":")
//                   << error.errorString();

        switch( error.error() )
        {
        case QSslError::CertificateUntrusted:
        case QSslError::SelfSignedCertificate:
        case QSslError::SelfSignedCertificateInChain:
            // TODO - remove HostNameMistchmach
            ignoreErrors << error;
            break;
        default:
            ignoreErrors << error;
            break;
        }
    }
    reply->ignoreSslErrors(ignoreErrors);

}


void
YaClient::prepareRequest(QNetworkRequest* request)
{
    QString accessToken = _settings.value("accessToken").toString();

    request->setRawHeader("Accept", "*/*");
    request->setRawHeader("X-Requested-With", "XMLHttpRequest");
    request->setHeader(QNetworkRequest::UserAgentHeader, "Yandex-Music-API");
    request->setRawHeader("X-Yandex-Music-Client", "YandexMusicAndroid/23020251");

    if (accessToken.size() > 0) {
        request->setRawHeader("Authorization", QString("OAuth %1").arg(accessToken).toLatin1());
    }
}

void
YaClient::requestAuth(QString username, QString password, QString captchaAnswer)
{
    QUrl url(AUTH_URL);
    QNetworkRequest r(url);
    QUrlQuery q;

    q.addQueryItem("grant_type", "password");
    q.addQueryItem("username", username);
    q.addQueryItem("password", password);
    q.addQueryItem("client_id", CLIENT_ID);
    q.addQueryItem("client_secret", CLIENT_SECRET);

    if (_captchaKey.length() != 0 && captchaAnswer.length() != 0) {
        q.addQueryItem("x_captcha_key", _captchaKey);
        q.addQueryItem("x_captcha_answer", captchaAnswer);
    }

    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    prepareRequest(&r);

    QNetworkReply* reply = _transport->post(r, q.toString().toLatin1());

    connect(reply, &QNetworkReply::finished, this, &YaClient::handleAuthResponse);

}

void
YaClient::setError(YaClient::Error code)
{
    _lastError = code;
    emit error(code);
}

void
YaClient::clearCaptchaData()
{
    _captchaKey = "";
    _captchaUrl = "";
}

void
YaClient::setCaptchaData(QString captchaKey, QString captchaUrl)
{
    _captchaKey = captchaKey;
    _captchaUrl = captchaUrl;
}


void
YaClient::handleAuthResponse()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    const QByteArray data = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        if (data.length() == 0) {
            setError(UndefinedError);
            return;
        }
        QJsonObject data_obj = QJsonDocument::fromJson(data).object();

        _lastErrorString = data;

        if (data_obj.value("error_description").toString() == "CAPTCHA required") {
            setCaptchaData(data_obj.value("x_captcha_key").toString(), data_obj.value("x_captcha_url").toString());
            setError(CaptchaRequired);
        } else if (data_obj.value("error_description").toString() == "login or password is not valid") {
            setError(InvalidCredentials);
        } else {
            setError(UndefinedError);
        }
        return;
    }

    // clear captchaKey and captchaUrl as we have passed through the check.
    clearCaptchaData();

    QJsonDocument data_doc = QJsonDocument::fromJson(data);
    QJsonObject data_obj = data_doc.object();

    if (!data_obj.contains("access_token")) {
        setError(UndefinedError);
        return;
    }

    _userId = QString::number(data_obj.value("uid").toInt());
    _token = data_obj.value("access_token").toString();
    _ttl = QDateTime::currentDateTime().addSecs(data_obj.value("expires_in").toInt());

    _settings.setValue("accessToken", _token);
    _settings.setValue("userId", _userId);
    _settings.setValue("ttl", _ttl.toString());

    emit authorized(_userId, _token);

    reply->deleteLater();
}
