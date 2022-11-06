#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QDataStream>

#include "models/RadioListModel.h"

RadioListModel::RadioListModel(QObject *parent) : PlayListModel(parent)
{
    _currentIndex = -1;
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

    QString strFromObj = QJsonDocument(data).toJson(QJsonDocument::Compact).toStdString().c_str();
    QByteArray dataEncoded = strFromObj.toUtf8();


    this->_transport->apiPostRequest(request, dataEncoded);
    connect(this->_transport, &ApiRequest::thirdPartyDataReady, this, &RadioListModel::handleFeedbackResponse);
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

void
RadioListModel::changeStation(QString station)
{
    if (_station != station) {
        qDebug() << "Changed station from " << _station << " to " << station << ". Clearing tracks...";
        _station = station;
        this->_tracks.clear();
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

    this->_transport->apiGetRequest(request);

    connect(this->_transport, &ApiRequest::dataReady, this, &PlayListModel::handleTracksResponse);

}

//void
//RadioListModel::handleTracksResponse(QJsonValue& data)
//{
////    qDebug() << data.toString();
//    QString dataStr = data.toString();
//    qint64 dataHash = hash(dataStr);

//    if (dataHash == _previousTracksValue) {
//        return;
//    } else {
//        _previousTracksValue = dataHash;
//    }

//    QJsonObject qjo = data.toObject();
//    QJsonArray tracks = qjo["sequence"].toArray();
//    _batchId = qjo["batchId"].toString();
//    // beginInsertRows(QModelIndex(), m_playList.count(), m_playList.count()+tracks.count()-1);

//    foreach (const QJsonValue& value, tracks) {
//        QJsonObject trackObject = value.toObject()["track"].toObject();
//        QJsonObject albumObject = trackObject["albums"].toArray().at(0).toObject();

//        Album* album = new Album;
//        album->id = QString::number(albumObject.value("id").toInt());
//        album->title = albumObject.value("title").toString();
//        album->coverUrl = "https://" + albumObject.value("coverUri").toString();

//        Track* newTrack = new Track;
//        newTrack->id = trackObject["id"].toString();
//        newTrack->title = trackObject["title"].toString();
//        newTrack->album = album;

//        newTrack->coverUrl = "https://" + trackObject["coverUri"].toString();
////        newTrack->cacheTrackCover("1000x1000");

//        for (QJsonValue&& artistObject : trackObject["artists"].toArray()) {
//            Artist* artist = new Artist;
//            artist->id = QString::number(artistObject.toObject()["id"].toInt());
//            artist->name = artistObject.toObject()["name"].toString();
//            artist->avatarUrl = "https://" + artistObject.toObject()["cover"].toObject()["uri"].toString();
//            newTrack->artists.append(QVariant::fromValue(artist));
//        }

//        beginInsertRows(QModelIndex(), rowCount(), rowCount());
//        _tracks.append(QVariant::fromValue(newTrack));
//        endInsertRows();

//    }

//    // weird
//    if (tracks.count() != 0) {
//        qDebug() << "Total tracks " << rowCount();
//        qDebug() << "Current index is " << _currentIndex << ". Changing to -1...";
//        _currentIndex = -1;
//        qDebug() << "Index now " << _currentIndex;
//        emit tracksReceived();
//    }
//}

void
RadioListModel::sendSettingsRequest(QString moodEnergy, QString diversity, QString language, QString type)
{

}

