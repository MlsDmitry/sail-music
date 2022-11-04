#ifndef TRACK_H
#define TRACK_H

#include <QMetaType>
#include <QString>
#include <QObject>
#include <QJsonValue>
#include <QVariantMap>
#include <QNetworkReply>
#include <QThread>

#include "utils/Cache.h"
#include "utils/Core.h"
#include "utils/ApiRequest.h"
#include "units/Album.h"

class Track : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString coverUrl MEMBER coverUrl NOTIFY coverUrlUpdated)

    explicit Track(QObject *parent = nullptr);

    QString id;
    QString title;
    QString coverUrl;

    Q_INVOKABLE void requestDownloadInfo();
    Q_INVOKABLE void requestFileDownloadInfo(QString bitrate);
    Q_INVOKABLE QString getDirectDownloadLink(QString bitrate);
    void cacheTrackCover(QString imageResolution);


public slots:
    void handleFileDownloadInfoResponse(QNetworkReply* reply);
    void handleDownloadInfoResponse(QJsonValue& data);

    void trackCoverSaved(QString imagePath, QString urlHash);
    void trackCoverDataReady(QNetworkReply* reply);

signals:
    void coverUrlUpdated();
    void downloadInfoReady();
    void downloadLinkReady(QString fileLink);

public:
    quint64 duration;
    quint64 totalPlayedSeconds;
    Album*      album;
    QVariantList artists;

private:
    Cache*      _cache;
    QThread*    _cacheThread;
    QString     _lastBitrate;
    QVariantMap _downloadInfos;
    QVariantMap _downloadDirectLinks;
    ApiRequest* _transport;

};

Q_DECLARE_METATYPE(Track*)

#endif // TRACK_H
