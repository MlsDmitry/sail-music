#ifndef RADIOLISTMODEL_H
#define RADIOLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QJsonValue>

#include "Models/PlayListModel.h"
#include "Utils/ApiRequest.h"
#include "Utils/Core.h"
#include "Models/ArtistModel.h"
#include "Models/AlbumModel.h"
#include "Models/TrackModel.h"
#include "Radio/RadioService.h"
#include "Radio/RadioAdapter.h"

#define RADIO_STARTED   "radioStarted"
#define TRACK_STARTED   "trackStarted"
#define TRACK_FINISHED  "trackFinished"
#define SKIP            "skip"

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

    virtual void getTracks() override;
    Q_INVOKABLE void sendFeedbackRequest(RadioListModel::FeedbackTypes feedbackType);
    Q_INVOKABLE void sendPlayAudioRequest();
    Q_INVOKABLE void sendInfoRequest();
    Q_INVOKABLE void sendSettingsRequest(QString moodEnergy, QString diversity, QString language, QString type);

public slots:
    void handleFeedbackResponse(QNetworkReply* reply);

signals:
    void feedbackReceived();
private:
    RadioService* _service;
};
#endif // RADIOLISTMODEL_H
