#ifndef RADIOADAPTER_H
#define RADIOADAPTER_H

#include "Adapters/TrackAdapter.h"

#include "json_dto/pub.hpp"

#include <QObject>
#include <QJsonValue>
#include <QVariantList>

struct feedback_t
{

    feedback_t () {}

    feedback_t (
            std::string station,
            std::string type,
//            std::string from,
            std::string timestamp
            )
    {
        _station = std::move(station);
        _type = std::move(type);
//        _from = std::move(from);
        _timestamp = std::move(timestamp);
    }

    std::string _station;
    std::string _type;
//    std::string _from;
    std::string _timestamp;
    json_dto::nullable_t<std::int64_t> _trackId {};
    json_dto::nullable_t<std::int64_t> _totalPlayedSeconds {};

    template<typename Json_Io>
    void json_io(Json_Io & io)
    {
        io & json_dto::mandatory("station", _station)
                & json_dto::mandatory("type", _type)
                & json_dto::optional("trackId", _trackId, nullptr)
                & json_dto::optional("totalPlayedSeconds", _totalPlayedSeconds, nullptr)
//                & json_dto::mandatory("from", _from)
                & json_dto::mandatory("timestamp", _timestamp);
    }
};


QVariantList
parseTracks(QJsonValue& data);

QString
createFeedbackRequest(QString station, QString type, QString trackId="0", int totalPlayedSeconds=0);


#endif // RADIOADAPTER_H
