#ifndef TRACK_H
#define TRACK_H

#include <QMetaType>
#include <QString>
#include <QObject>
#include <QJsonValue>
#include <QVariantMap>
#include <QNetworkReply>
#include <QThread>

#include "Utils/Cache.h"
#include "Utils/Core.h"
#include "Services/TrackService.h"
#include "Models/AlbumModel.h"

class Track : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString coverUrl MEMBER coverUrl NOTIFY coverUrlUpdated)
    Q_PROPERTY(QString title MEMBER title)

    explicit Track(QObject *parent = nullptr);

    Q_INVOKABLE void getTrackDownloadInfo();
    Q_INVOKABLE void getTrackFileDownloadLink(QString bitrate);
    Q_INVOKABLE QString getDirectDownloadLink(QString bitrate);
    Q_INVOKABLE QString getMaxBitrateAvailable();
//    void cacheTrackCover(QString imageResolution);



signals:
    void coverUrlUpdated();
    void downloadInfoReady();
    void downloadLinkReady(QString fileLink);

public:
    QString id;
    QString title;
    QString coverUrl;

    quint64 duration;
    quint64 totalPlayedSeconds;
    Album*      album;
    QVariantList artists;

    QVariantMap downloadInfos;
//    quint16     minBitrate;
    quint16 maxBitrate;
private:
    Cache*      _cache;
    QThread*    _cacheThread;
    QString     _lastBitrate;
    QVariantMap _downloadDirectLinks;
    TrackService* _service;

};

Q_DECLARE_METATYPE(Track*)

#endif // TRACK_H
