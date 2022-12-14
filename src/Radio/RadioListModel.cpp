    #include <QCryptographicHash>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrlQuery>


#include "Radio/RadioListModel.h"
#include "Utils/commons.h"

RadioListModel::RadioListModel(QObject *parent) : PlayListModel(parent)
{
    _service = new RadioService();
}

void RadioListModel::sendPlayAudioRequest()
{
}

void RadioListModel::sendFeedbackRequest(RadioListModel::FeedbackTypes feedbackType)
{
    QString type;

    switch (feedbackType)
    {
    case RadioStarted:
        type = RADIO_STARTED;
        break;
    case TrackFinished:
        type = TRACK_FINISHED;
        break;
    case TrackStarted:
        type = TRACK_STARTED;
        break;
    case Skip:
        type = SKIP;
    }
    QString data;

    if (_currentIndex == -1)
        data = createFeedbackRequest(_station, type, "0", 0);
    else
        data = createFeedbackRequest(_station, type, _currentTrack->id, _currentTrack->totalPlayedMs / 1000);

    _service->requestFeedback(_station, _batchId, data);
}

void RadioListModel::handleFeedbackResponse(QNetworkReply *reply)
{
    reply->deleteLater();

    emit feedbackReceived();
}

void RadioListModel::sendInfoRequest()
{
}

void RadioListModel::changeStation(QString station)
{
    if (_station != station)
    {
        qDebug() << "Changed station from " << _station << " to " << station;
        _station = station;
//        this->_tracks.clear();
    }
}

void RadioListModel::getTracks()
{
    _service->requestTracks(_station);

    connect(
        _service, &RadioService::tracksReceived, this,
        [=](QJsonValue &data) {

            QVariantList newTracks = parseTracks(data);
            QString batchId = parseBatchId(data);

            if (newTracks.count() == 0)
                return;

            qint64 newHash = hash(QJsonDocument(data.toObject()["sequence"].toArray()).toJson(QJsonDocument::Compact));

            if (newHash == _prevTracksHash) {
                _prevTracksHash = newHash;
                return;
            }
            _prevTracksHash = newHash;

            beginInsertRows(QModelIndex(), rowCount(), rowCount() + newTracks.count() - 1);
            for (auto track : newTracks) {
//                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                qDebug() << "Track info: " << track.value<Track*>()->title;
                _tracks.push_back(track);
                qDebug() << "Tracks size: " << _tracks.size();
//                endInsertRows();
            }
            endInsertRows();

            _batchId = batchId;

            if (_currentIndex == -1)
                setIndex(0);

            emit tracksReceived();
        },
        Qt::UniqueConnection);

}

void RadioListModel::sendSettingsRequest(QString moodEnergy, QString diversity, QString language, QString type)
{
}
