#include "RadioAdapter.h"
#include "Adapters/TrackAdapter.h"
#include "Adapters/AlbumAdapter.h"
#include "Models/TrackModel.h"

#include <iostream>
#include <string>

#include <QVariantMap>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QString>

QVariantList
parseTracks(QJsonValue& jsonData)
{
    QVariantList tracks;

    QJsonArray tracksJson = jsonData.toObject().value("sequence").toArray();

    for (QJsonValue trackJson : tracksJson)
    {
        QJsonValue track = trackJson.toObject().value("track");
        QJsonValue album = track.toObject()["albums"].toArray().at(0);

        Track* newTrack = parseTrack(track);

        newTrack->album = parseAlbum(album);

        tracks.append(QVariant::fromValue(newTrack));

    }

    return tracks;
}



QString
createFeedbackRequest(QString station, QString type, QString trackId, int totalPlayedSeconds)
{
    feedback_t feedback {
        station.toStdString(),
                type.toStdString(),
//                from.toStdString(),
                QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ").toStdString()
    };

    if (trackId.toInt() != 0)
        feedback._trackId = trackId.toInt();

    if (totalPlayedSeconds != 0)
        feedback._totalPlayedSeconds = totalPlayedSeconds;

    return QString::fromStdString(json_dto::to_json<feedback_t> (feedback));
}
