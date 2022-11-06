#ifndef TRACKADAPTER_H
#define TRACKADAPTER_H

#include "json_dto/pub.hpp"

#include <iostream>
#include <string>

#include <QVariantMap>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QString>
#include <QObject>

struct track_t
{

    track_t () {}

    track_t (
            std::string id,
            std::string title,
            std::string coverUrl,
            std::uint64_t durationMs)
    {
        _id = std::move(id);
        _title = std::move(title);
        _coverUrl = std::move(coverUrl);
        _durationMs = durationMs;
    }

    std::string _id;
    std::string _title;
    std::string _coverUrl;
    std::uint64_t _durationMs;

    template<typename Json_Io>
    void json_io(Json_Io & io)
    {
        io & json_dto::mandatory("id", _id)
            & json_dto::mandatory("title", _title)
            & json_dto::mandatory("coverUri", _coverUrl)
            & json_dto::mandatory("durationMs", _durationMs);
    }
};


void parseTrack(QJsonValue& data);

#endif // TRACKADAPTER_H
