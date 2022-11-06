#ifndef TRACKSERVICE_H
#define TRACKSERVICE_H

#include "Utils/ApiRequest.h"

#include <QObject>
#include <QJsonValue>

#define TRACK_DOWNLOAD_INFO "/tracks/%s/download-info"

class TrackService : public QObject
{
    Q_OBJECT
public:
    explicit TrackService(QObject *parent = nullptr);

    void requestDownloadInfo(QString id);
    void requestFileDownloadInfo(QString downloadInfoUrl);

signals:
    void trackDownloadInfoReady(QJsonValue& data);
    void trackFileDownloadInfoReady(QByteArray data);

private:
    ApiRequest* _transport;

};

#endif // TRACKSERVICE_H
