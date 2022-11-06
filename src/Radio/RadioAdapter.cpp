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
        QJsonValue track = trackJson.toObject().value("track");

        tracks.append(QVariant::fromValue(parseTrack(track)));
    }

    return tracks;
}
