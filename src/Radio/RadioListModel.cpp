#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QDataStream>

#include "Radio/RadioListModel.h"
#include "Utils/commons.h"

RadioListModel::RadioListModel(QObject *parent) : PlayListModel(parent)
{
    _currentIndex = -1;
    _service = new RadioService();
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
    QUrl url(makeUrl(MUSIC_API_URL, STATION_FEEDBACK, _station.toStdString().data()));

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
    connect(this->_transport, &ApiRequest::thirdPartyDataReady, this, &RadioListModel::handleFeedbackResponse, Qt::UniqueConnection);
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
RadioListModel::getTracks()
{
    _service->requestTracks(_station);

    connect(_service, &RadioService::tracksReceived, this, [=](QJsonValue& data){
        QVariantList newTracks = parseTracks(data);

        beginInsertRows(QModelIndex(), rowCount(), rowCount() + newTracks.count());
        _tracks.append(newTracks);
        endInsertRows();

        emit tracksReceived();
    }, Qt::UniqueConnection);
}

void
RadioListModel::sendSettingsRequest(QString moodEnergy, QString diversity, QString language, QString type)
{

}

