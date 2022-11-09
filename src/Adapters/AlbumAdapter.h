#ifndef ALBUMADAPTER_H
#define ALBUMADAPTER_H

#include "Models/AlbumModel.h"

#include "json_dto/pub.hpp"

#include <string>

#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>

struct album_t
{

    album_t () {}

    album_t (
            std::int64_t id,
            std::string title,
            std::string coverUrl)
    {
        _id = id;
        _title = std::move(title);
        _coverUrl = std::move(coverUrl);
    }

    std::int64_t    _id;
    std::string      _title;
    std::string     _coverUrl;

    template<typename Json_Io>
    void json_io(Json_Io & io)
    {
        io & json_dto::mandatory("id", _id)
                & json_dto::mandatory("title", _title)
                & json_dto::mandatory("coverUri", _coverUrl);
    }
};

Album* parseAlbum(QJsonValue& data);

#endif // ALBUMADAPTER_H
