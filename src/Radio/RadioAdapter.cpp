#include "RadioAdapter.h"
#include "Adapters/TrackAdapter.h"
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

        QString trackStr = QJsonDocument(trackJson.toObject().value("track").toObject()).toJson(QJsonDocument::Compact);
        auto track = json_dto::from_json<track_t>(trackStr.toStdString());

        Track* trackModel = new Track;

        trackModel->id = QString::fromStdString(track._id);
        trackModel->title = QString::fromStdString(track._title);
        trackModel->coverUrl = "https://" + QString::fromStdString(track._coverUrl);

        tracks.append(QVariant::fromValue(trackModel));
    }

    return tracks;
}
