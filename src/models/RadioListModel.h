#ifndef RADIOLISTMODEL_H
#define RADIOLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QJsonValue>

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

class RadioListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_PROPERTY(QString station MEMBER _station)
    Q_PROPERTY(qint64 currentIndex MEMBER _currentIndex)
    Q_PROPERTY(Track* currentTrack MEMBER _currentTrack)

    explicit RadioListModel(QObject *parent = nullptr);

    enum FeedbackTypes {
        RadioStarted,
        TrackStarted,
        TrackFinished,
        Skip
    };

    Q_ENUM(FeedbackTypes)

    enum Roles {
        TITLE = Qt::UserRole + 1,
        COVER_URL
    };

    Q_ENUM(Roles)

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    QString makeUrl(const char *format, ...);

    Q_INVOKABLE QVariant get(int idx);
    Q_INVOKABLE void setIndex(int idx);
    Q_INVOKABLE void changeStation(QString station);
    Q_INVOKABLE void prepareCurrentTrackToPlay();

//    Q_INVOKABLE void sendDownloadInfoRequest(int id);

    Q_INVOKABLE void sendTracksRequest();
    Q_INVOKABLE void sendFeedbackRequest(RadioListModel::FeedbackTypes feedbackType);
    Q_INVOKABLE void sendPlayAudioRequest();
    Q_INVOKABLE void sendInfoRequest();
    Q_INVOKABLE void sendSettingsRequest(QString moodEnergy, QString diversity, QString language, QString type);

public slots:
//    void handleDownloadInfoResponse(QJsonValue& data);
    void trackDownloadInfoReady();
    void handleTracksResponse(QJsonValue& reply);
    void handleFeedbackResponse(QNetworkReply* reply);

signals:
    void currentTrackLinkReady(QString url);
    void tracksReceived();
    void feedbackReceived();

private:
    ApiRequest* _transport;
    QString _station;
    QString _batchId;
    QString _trackId;
    QVariantList _tracks;

    qint64 _previousTracksValue;
    qint64  _currentIndex;
    Track*  _currentTrack;
};

#endif // RADIOLISTMODEL_H
