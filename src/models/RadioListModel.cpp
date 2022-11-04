#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QDataStream>

#include "models/RadioListModel.h"

RadioListModel::RadioListModel(QObject *parent) : QAbstractListModel(parent)
{
    _transport = new ApiRequest;
    _currentIndex = -1;

}

int RadioListModel::rowCount(const QModelIndex&) const{
    return _tracks.count();
}

QHash<int,QByteArray>
RadioListModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    //    roles.insert(RadioListModel::ID, "id");
    roles.insert(RadioListModel::TITLE, "title");
    roles.insert(RadioListModel::COVER_URL, "coverUrl");

    return roles;
}


QVariant
RadioListModel::data(const QModelIndex &index, int role) const {
    Q_UNUSED(role)
    if (!index.isValid())
        return QVariant();

    int id = QMetaType::type("Track");
    if (id == QMetaType::UnknownType)
        return QVariant();

    Track* track = _tracks.at(index.row()).value<Track*>();
    switch (role) {
    //        case RadioListModel::ID:
    //            return track.id;
    //            break;
    case RadioListModel::TITLE:
        return track->title;
        break;
    case RadioListModel::COVER_URL:
        return track->coverUrl;
        break;
    }
    return QVariant();
}

void
RadioListModel::sendPlayAudioRequest()
{

//    QUrlQuery query;
//    Settings settings;
//    QDateTime current = QDateTime::currentDateTime();
//    QString curdt = current.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
//    QString userId = settings.value("userId").toString();
//    query.addQueryItem("uid", userId);
//    query.addQueryItem("client-now", curdt);
//    query.addQueryItem("from-cache", "false");
//    query.addQueryItem("track-id", _currentTrack->id);
//    query.addQueryItem("track-length-seconds", QString::number(_currentTrack->duration));
//    query.addQueryItem("end-position-seconds", QString::number(_currentTrack->duration));
//    query.addQueryItem("from", "mobile-home-rup_main-user-onyourwave-default");
//    query.addQueryItem("album-id", _currentTrack->blockSignals()));
//    query.addQueryItem("play-id", "79CFB84C-4A0B-4B31-8954-3006C0BD9274");
//    query.addQueryItem("timestamp", curdt);
//    query.addQueryItem("total-played-seconds", QString::number(m_playList.at(m_currentIndex)->duration));
//    qDebug() << query.toString();
//    m_api->makeApiPostRequest("/play-audio?" + query.toString(), QString(""));
}



void
RadioListModel::sendFeedbackRequest(RadioListModel::FeedbackTypes feedbackType)
{
    QString type;
    QJsonObject data;
    QNetworkRequest request;
    QDateTime currentDt = QDateTime::currentDateTime();
    QUrl url(makeUrl(STATION_FEEDBACK, _station.toStdString().data()));

    QString currentDtStr = currentDt.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
    url.setQuery("batch-id=" + _batchId);
    request.setUrl(url);



    switch (feedbackType) {
    case RadioStarted:
        data.insert("type", RADIO_STARTED);
        break;
    case TrackFinished:
        data.insert("type", TRACK_FINISHED);
        break;
    case TrackStarted:
        data.insert("type", TRACK_STARTED);
        break;
    case Skip:
        data.insert("type", SKIP);
    }

    data.insert("timestamp", currentDtStr);

    //    { "totalPlayedSeconds", QString::number(m_playList.at(m_currentIndex)->duration) },
    //        { "trackId", QString::number(m_playList.at(m_currentIndex)->trackId) + ":" + QString::number(m_playList.at(m_currentIndex)->albumCoverId) }
    //    };
    QString strFromObj = QJsonDocument(data).toJson(QJsonDocument::Compact).toStdString().c_str();
//    qDebug() << "JSON: " << strFromObj;
    QByteArray dataEncoded = strFromObj.toUtf8();


    _transport->apiPostRequest(request, dataEncoded);
    connect(_transport, &ApiRequest::thirdPartyDataReady, this, &RadioListModel::handleFeedbackResponse);
    //    m_api->makeApiPostRequest("/rotor/station/user:onyourwave/feedback?batch-id=" + batchid, strFromObj);
}

void
RadioListModel::handleFeedbackResponse(QNetworkReply* reply)
{
    reply->deleteLater();

    emit feedbackReceived();
}

void
RadioListModel::sendInfoRequest()
{

}

QString
RadioListModel::makeUrl(const char *format, ...)
{
    QString url(MUSIC_API_URL);

    va_list args;
    va_start(args, format);

    QString url2 = QString().vsprintf(format, args);

    url.append(url2);

    va_end(args);

    return url;
}

void
RadioListModel::changeStation(QString station)
{
    if (_station != station) {
        qDebug() << "Changed station from " << _station << " to " << station << ". Clearing tracks...";
        _station = station;
        _tracks.clear();
    }
}


void
RadioListModel::sendTracksRequest()
{
    QNetworkRequest request;
    QUrlQuery query;
    QUrl url(makeUrl(STATION_TRACKS, _station.toStdString().data()));

    query.addQueryItem("settings2", "true");

    url.setQuery(query);

    request.setUrl(url);

    _transport->apiGetRequest(request);

    connect(_transport, &ApiRequest::dataReady, this, &RadioListModel::handleTracksResponse);

}

qint64 hash(const QString & str)
{
    QByteArray hash = QCryptographicHash::hash(
                QByteArray::fromRawData((const char*)str.utf16(), str.length()*2),
                QCryptographicHash::Md5
                );
    Q_ASSERT(hash.size() == 16);
    QDataStream stream(hash);
    qint64 a, b;
    stream >> a >> b;
    return a ^ b;
}

void
RadioListModel::handleTracksResponse(QJsonValue& data)
{
//    qDebug() << data.toString();
    QString dataStr = data.toString();
    qint64 dataHash = hash(dataStr);

    if (dataHash == _previousTracksValue) {
        return;
    } else {
        _previousTracksValue = dataHash;
    }

    QJsonObject qjo = data.toObject();
    QJsonArray tracks = qjo["sequence"].toArray();
    _batchId = qjo["batchId"].toString();
    // beginInsertRows(QModelIndex(), m_playList.count(), m_playList.count()+tracks.count()-1);

    foreach (const QJsonValue& value, tracks) {
        QJsonObject trackObject = value.toObject()["track"].toObject();
        QJsonObject albumObject = trackObject["albums"].toArray().at(0).toObject();

        Album* album = new Album;
        album->id = QString::number(albumObject.value("id").toInt());
        album->title = albumObject.value("title").toString();
        album->coverUrl = "https://" + albumObject.value("coverUri").toString();

        Track* newTrack = new Track;
        newTrack->id = trackObject["id"].toString();
        newTrack->title = trackObject["title"].toString();
        newTrack->album = album;

        newTrack->coverUrl = "https://" + trackObject["coverUri"].toString();
        newTrack->cacheTrackCover("1000x1000");

        for (QJsonValue&& artistObject : trackObject["artists"].toArray()) {
            Artist* artist = new Artist;
            artist->id = QString::number(artistObject.toObject()["id"].toInt());
            artist->name = artistObject.toObject()["name"].toString();
            artist->avatarUrl = "https://" + artistObject.toObject()["cover"].toObject()["uri"].toString();
            newTrack->artists.append(QVariant::fromValue(artist));
        }

        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        _tracks.append(QVariant::fromValue(newTrack));
        endInsertRows();

    }

    // weird
    if (tracks.count() != 0) {
        qDebug() << "Total tracks " << rowCount();
        qDebug() << "Current index is " << _currentIndex << ". Changing to -1...";
        _currentIndex = -1;
        qDebug() << "Index now " << _currentIndex;
        emit tracksReceived();
    }
}

void
RadioListModel::sendSettingsRequest(QString moodEnergy, QString diversity, QString language, QString type)
{

}

QVariant
RadioListModel::get(int idx)
{
    if (idx >= _tracks.count() || idx < 0) {
        return QVariant();
    }

    QMap<QString, QVariant> itemData;

    Track *track = _tracks.at(idx).value<Track*>();

    itemData.insert("title", track->title);
    itemData.insert("coverUrl", track->coverUrl);

    return itemData;
}

void
RadioListModel::setIndex(int idx)
{
    if (idx >= 0 && idx < _tracks.count()) {
        qDebug() << "Set _currentIndex to " << idx;
        _currentIndex = idx;
        _currentTrack = _tracks.at(idx).value<Track*>();
    }
}

void
RadioListModel::prepareCurrentTrackToPlay()
{

    _currentTrack->requestDownloadInfo();

    connect(_currentTrack, &Track::downloadInfoReady, this, &RadioListModel::trackDownloadInfoReady);
}

void
RadioListModel::trackDownloadInfoReady()
{
    _currentTrack->requestFileDownloadInfo("320");

    connect(_currentTrack, &Track::downloadLinkReady, this, &RadioListModel::currentTrackLinkReady);
}
