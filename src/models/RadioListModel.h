#ifndef RADIOLISTMODEL_H
#define RADIOLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QJsonValue>

#include "models/PlayListModel.h"
#include "utils/ApiRequest.h"
#include "utils/Core.h"
#include "units/Artist.h"
#include "units/Album.h"
#include "units/Track.h"

#define RADIO_STARTED   "radioStarted"
#define TRACK_STARTED   "trackStarted"
#define TRACK_FINISHED  "trackFinished"
#define SKIP            "skip"

#define STATION_FEEDBACK    "/rotor/station/%s/feedback"
#define STATION_TRACKS      "/rotor/station/%s/tracks"
#define STATION_INFO        "/rotor/station/%s/info"
#define STATION_SETTINGS    "/rotor/station/%s/settings2"

class RadioListModel : public PlayListModel
{
    Q_OBJECT
public:

    explicit RadioListModel(QObject *parent = nullptr);

    enum FeedbackTypes {
        RadioStarted,
        TrackStarted,
        TrackFinished,
        Skip
    };

    Q_ENUM(FeedbackTypes)

//    enum Roles {
//        TITLE = Qt::UserRole + 1,
//        COVER_URL
//    };

//    Q_ENUM(Roles)

    Q_INVOKABLE void changeStation(QString station);

    virtual void sendTracksRequest() override;
    Q_INVOKABLE void sendFeedbackRequest(RadioListModel::FeedbackTypes feedbackType);
    Q_INVOKABLE void sendPlayAudioRequest();
    Q_INVOKABLE void sendInfoRequest();
    Q_INVOKABLE void sendSettingsRequest(QString moodEnergy, QString diversity, QString language, QString type);

public slots:
    void handleFeedbackResponse(QNetworkReply* reply);

signals:
    void feedbackReceived();
};

#endif // RADIOLISTMODEL_H
