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
    _currentIndex = -1;
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
        qDebug() << "Changed station from " << _station << " to " << station << ". Clearing tracks...";
        _station = station;
        this->_tracks.clear();
    }
}

void RadioListModel::getTracks()
{
    _service->requestTracks(_station);

    connect(
        _service, &RadioService::tracksReceived, this,
        [=](QJsonValue &data) {
            QVariantList newTracks = parseTracks(data);

            beginInsertRows(QModelIndex(), rowCount(), rowCount() + newTracks.count());
            _tracks.append(newTracks);
            endInsertRows();

            emit tracksReceived();
        },
        Qt::UniqueConnection);
}

void RadioListModel::sendSettingsRequest(QString moodEnergy, QString diversity, QString language, QString type)
{
}
