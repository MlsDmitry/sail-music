#ifndef TRACK_H
#define TRACK_H

#include <QJsonValue>
#include <QMetaType>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QThread>
#include <QVariantMap>

#include "Models/AlbumModel.h"
#include "Services/TrackService.h"
#include "Utils/Cache.h"
#include "Utils/Core.h"

class Track : public QObject
{
    Q_OBJECT
  public:
    Q_PROPERTY(QString coverUrl MEMBER coverUrl NOTIFY coverUrlUpdated)
    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(Album *album MEMBER album)
    Q_PROPERTY(qint64 duration MEMBER duration)
    Q_PROPERTY(qint64 totalPlayedMs MEMBER totalPlayedMs);

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

    qint64 duration;
    qint64 totalPlayedMs;
    Album *album;
    QVariantList artists;

    QVariantMap downloadInfos;
    //    quint16     minBitrate;
    quint16 maxBitrate;

  private:
    Cache *_cache;
    QThread *_cacheThread;
    QString _lastBitrate;
    QVariantMap _downloadDirectLinks;
    TrackService *_service;
};

Q_DECLARE_METATYPE(Track *)

#endif // TRACK_H
