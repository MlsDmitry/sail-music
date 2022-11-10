#ifndef PLAYLISTSERVICE_H
#define PLAYLISTSERVICE_H

#include "Utils/ApiRequest.h"

#include <QObject>

class PlaylistService : public QObject
{
    Q_OBJECT
  public:
    explicit PlaylistService(QObject *parent = nullptr);

    Q_INVOKABLE virtual void requestTracks();

  public slots:
    //    void handleTracksResponse(QJsonValue& reply);
    //    void trackDownloadInfoReady();

  signals:
    void currentTrackLinkReady(QString url);
    void tracksReceived();
    void currentTrackUpdated();

  protected:
    ApiRequest *_transport;
};

#endif // PLAYLISTSERVICE_H
