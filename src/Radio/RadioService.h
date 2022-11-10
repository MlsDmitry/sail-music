#ifndef RADIOSERVICE_H
#define RADIOSERVICE_H

#include "Services/PlaylistService.h"

#include <QObject>
#include <QString>

#define STATION_FEEDBACK "/rotor/station/%s/feedback"
#define STATION_TRACKS "/rotor/station/%s/tracks"
#define STATION_INFO "/rotor/station/%s/info"
#define STATION_SETTINGS "/rotor/station/%s/settings2"

class RadioService : public PlaylistService
{
    Q_OBJECT
  public:
    explicit RadioService(QObject *parent = nullptr);

    virtual void requestTracks(QString station);
    void requestFeedback(QString station, QString batchId, QString &feedbackData);
  signals:
    void tracksReceived(QJsonValue &data);
};

#endif // RADIOSERVICE_H
