#ifndef YACLIENT_H
#define YACLIENT_H

#include <QDateTime>
#include <QJSEngine>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include <QQmlEngine>
#include <QString>

#include "Adapters/YaClientAdapter.h"
#include "Models/PlayListModel.h"
#include "Services/YaClientService.h"
#include "Utils/ApiRequest.h"
#include "Utils/Settings.h"

#define AUTH_URL "https://oauth.yandex.ru/token"
#define API_URL "https://api.music.yandex.net"
#define CLIENT_ID "23cabbbdc6cd418abb4b39c32c41195d"
#define CLIENT_SECRET "53bc75238f0c4d08a118e51fe9203300"

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
    Q_PROPERTY(PlayListModel *currentPlaylist MEMBER currentPlaylist NOTIFY playlistUpdated)
    Q_PROPERTY(QMediaPlayer *player MEMBER _player)

    explicit YaClient(QObject *parent = nullptr);

    enum Error
    {
        InvalidCredentials,
        CaptchaRequired,
        UndefinedError
    };

    Q_ENUM(Error)

    enum State
    {
        StoppedState,
        PlayingState,
        PausedState
    };

    Q_ENUM(State)

    enum MediaStatus
    {
        UnknownMediaStatus,
        NoMedia,
        LoadingMedia,
        LoadedMedia,
        StalledMedia,
        BufferingMedia,
        BufferedMedia,
        EndOfMedia,
        InvalidMedia
    };

    Q_ENUM(MediaStatus)

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new YaClient;
    }

    Q_INVOKABLE void requestAuth(QString username, QString password, QString captchaAnswer = "");

    Q_INVOKABLE void continuePlay();
    Q_INVOKABLE void play(QString url);
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void seek(qint64 position);
    Q_INVOKABLE QMediaPlayer::State playState();

  public slots:
    void replySSLErrors(QNetworkReply *reply, QList<QSslError> errors);
    void handleAuthResponse(QNetworkReply *reply);
    void audioPositionChanged(qint64 duration);
    void setPlaylist(PlayListModel* playlist);

  signals:
    void audioStateChanged(QMediaPlayer::State state);
    void audioProgress(qint64 position);
    void error(YaClient::Error error);
    void authorized(QString userId, QString token);
    void playlistUpdated();

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

    QMediaPlayer *_player;
    YaClientService *_service;

  public:
    PlayListModel *currentPlaylist;
    ApiRequest *_transport;
};

#endif // YACLIENT_H
