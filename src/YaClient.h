#ifndef YACLIENT_H
#define YACLIENT_H

#include <QObject>
#include <QString>
#include <QNetworkRequest>
#include <QQmlEngine>
#include <QJSEngine>
#include <QNetworkAccessManager>
#include <QDateTime>

#include "settings.h"
//#include "Core.h"

#define AUTH_URL        "https://oauth.yandex.ru/token"
#define CLIENT_ID       "23cabbbdc6cd418abb4b39c32c41195d"
#define CLIENT_SECRET   "53bc75238f0c4d08a118e51fe9203300"

class YaClient : public QObject
{
    Q_OBJECT
//    Q_DISABLE_COPY(YaClient)
public:
    Q_PROPERTY(QString captchaUrl MEMBER _captchaUrl)
    Q_PROPERTY(QString lastUsername MEMBER _lastUsername)
    Q_PROPERTY(QString lastPassword MEMBER _lastPassword)
    Q_PROPERTY(YaClient::Error lastError MEMBER _lastError)
    Q_PROPERTY(QString lastErrorString MEMBER _lastErrorString)

    explicit YaClient(QObject *parent = nullptr);

    enum Error {
        InvalidCredentials,
        CaptchaRequired,
        UndefinedError
    };

    Q_ENUM(Error)

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new YaClient;
    }

    Q_INVOKABLE void requestAuth(QString username, QString password, QString captchaAnswer="");


public slots:    
    void replySSLErrors(QNetworkReply *reply, QList<QSslError> errors);
    void handleAuthResponse();

signals:
    void error(YaClient::Error error);
    void authorized(QString token, QString userId);

private:
    void clearCaptchaData();
    void setCaptchaData(QString captchaKey, QString captchaUrl);
    void setError(YaClient::Error code);
    void prepareRequest(QNetworkRequest *request);

private:
    Settings _settings;

    YaClient::Error _lastError;
    QString _lastErrorString;

    QString _lastUsername;
    QString _lastPassword;

    QString _captchaKey;
    QString _captchaUrl;

    QString _userId;
    QString _token;
    QDateTime _ttl;

public:
    QNetworkAccessManager* _transport;


};

#endif // YACLIENT_H
