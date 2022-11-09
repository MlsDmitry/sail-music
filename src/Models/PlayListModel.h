#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QUrl>
#include <QNetworkRequest>

#include "Utils/ApiRequest.h"
#include "Utils/Core.h"
#include "Models/ArtistModel.h"
#include "Models/AlbumModel.h"
#include "Models/TrackModel.h"


class PlayListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_PROPERTY(QString station MEMBER _station)
    Q_PROPERTY(qint64 currentIndex MEMBER _currentIndex)
    Q_PROPERTY(Track* currentTrack MEMBER _currentTrack NOTIFY currentTrackUpdated)

    explicit PlayListModel(QObject *parent = nullptr);


    enum Roles {
        TITLE = Qt::UserRole + 1,
        COVER_URL
    };

    Q_ENUM(Roles)

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE QVariant get(int idx);
    Q_INVOKABLE void setIndex(int idx);
    Q_INVOKABLE void prepareCurrentTrackToPlay();

    Q_INVOKABLE virtual void getTracks();

    int getCurrentTrackId();
    int getCurrentAlbumId();
    void updateCurrentTrackPlayedSeconds(qint64 seconds);

public slots:
    void handleTracksResponse(QJsonValue& reply);
    void trackDownloadInfoReady();


signals:
    void currentTrackLinkReady(QString url);
    void tracksReceived();
    void currentTrackUpdated();

protected:
    ApiRequest* _transport;
    QString _station;
    QString _batchId;
    QString _trackId;
    QVariantList _tracks;

    qint64 _previousTracksValue;
    qint64  _currentIndex;
    Track*  _currentTrack;
};

#endif // PLAYLISTMODEL_H
