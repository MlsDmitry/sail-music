#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkProxy>
#include <QMediaPlayer>
#include <QSslError>
#include <QCoreApplication>

#include "Models/YaClientModel.h"

YaClient::YaClient(QObject *parent) : QObject(parent)
{
    _transport = new ApiRequest();
    _player = new QMediaPlayer();
    _service = new YaClientService();
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
YaClient::requestAuth(QString username, QString password, QString captchaAnswer)
{
    QUrl url(AUTH_URL);
    QNetworkRequest request(url);
    QUrlQuery query;
    QByteArray data;

    query.addQueryItem("grant_type", "password");
    query.addQueryItem("username", username);
    query.addQueryItem("password", password);
    query.addQueryItem("client_id", CLIENT_ID);
    query.addQueryItem("client_secret", CLIENT_SECRET);

    if (_captchaKey.length() != 0 && captchaAnswer.length() != 0) {
        query.addQueryItem("x_captcha_key", _captchaKey);
        query.addQueryItem("x_captcha_answer", captchaAnswer);
    }

    data = query.toString(QUrl::FullyEncoded).toUtf8();

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


    _transport->thirdPartyPostRequest(request, data, false);

    connect(_transport, &ApiRequest::thirdPartyDataReady, this, &YaClient::handleAuthResponse);

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
YaClient::handleAuthResponse(QNetworkReply* reply)
{
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


void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void
YaClient::play(QString url)
{
    if (_player->state() == QMediaPlayer::PlayingState)
        _player->stop();

    qDebug() << "Playing " << url;

    _player->setMedia(QUrl(url));
    _player->setVolume(100);
    _player->play();

    struct play_audio_request_info info;

    info.track_id = currentPlaylist->getCurrentTrackId();
    info.album_id = currentPlaylist->getCurrentAlbumId();


    QString data = createPlayAudioRequestData(info);

    _service->requestPlayAudio(data);

}

void
YaClient::continuePlay()
{
    _player->play();
}

void
YaClient::pause()
{
    _player->pause();
}

QMediaPlayer::State
YaClient::playState()
{
    return _player->state();
}

void
YaClient::stop()
{
    _player->stop();
}
