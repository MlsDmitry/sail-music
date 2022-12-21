#include "RadioAdapter.h"
#include "Adapters/AlbumAdapter.h"
#include "Adapters/TrackAdapter.h"
#include "Models/TrackModel.h"

#include <iostream>
#include <string>

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QVariantMap>

QVariantList parseTracks(QJsonValue &jsonData)
{
    QVariantList tracks;

    QJsonArray tracksJson = jsonData.toObject().value("sequence").toArray();

    for (QJsonValue trackJson : tracksJson)
    {
        QJsonValue track = trackJson.toObject().value("track");
        QJsonValue album = track.toObject()["albums"].toArray().at(0);

        auto newTrack = parseTrack(track);

        newTrack->album = parseAlbum(album);

        tracks.append(QVariant::fromValue(newTrack));
    }

    return tracks;
}

QString parseBatchId(QJsonValue &jsonData)
{
    return jsonData.toObject().contains("batchId") ? jsonData.toObject().value("batchId").toString() : "";
}

QString createFeedbackRequest(QString station, QString type, QString trackId, int totalPlayedSeconds)
{
    feedback_t feedback{station.toStdString(), type.toStdString(),
                        //                from.toStdString(),
                        QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ").toStdString()};

    if (trackId.toInt() != 0) {
        feedback._trackId = trackId.toInt();
        feedback._totalPlayedSeconds = totalPlayedSeconds;
    }

    return QString::fromStdString(json_dto::to_json<feedback_t>(feedback));
}
