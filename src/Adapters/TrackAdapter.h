#ifndef TRACKADAPTER_H
#define TRACKADAPTER_H

#include "Models/TrackModel.h"
#include "json_dto/pub.hpp"

#include <iostream>
#include <memory>
#include <string>

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QSharedPointer>

struct track_t
{

    track_t()
    {
    }

    track_t(std::string id, std::string title, std::string coverUrl, std::uint64_t durationMs)
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

    template <typename Json_Io> void json_io(Json_Io &io)
    {
        io &json_dto::mandatory("id", _id) & json_dto::mandatory("title", _title) &
            json_dto::mandatory("coverUri", _coverUrl) & json_dto::mandatory("durationMs", _durationMs);
    }
};

struct track_download_info_t
{
    track_download_info_t()
    {
    }

    track_download_info_t(std::string codec, std::string downloadInfoUrl, std::uint16_t bitrate)
    {
        _codec = std::move(codec);
        _downloadInfoUrl = std::move(downloadInfoUrl);
        bitrate = _bitrate;
    }

    std::string _codec;
    std::string _downloadInfoUrl;
    std::uint16_t _bitrate;

    template <typename Json_Io> void json_io(Json_Io &io)
    {
        io &json_dto::mandatory("codec", _codec) & json_dto::mandatory("downloadInfoUrl", _downloadInfoUrl) &
            json_dto::mandatory("bitrateInKbps", _bitrate);
    }
};

struct track_file_download_info
{
    QString path;
    QString signature;
    QString host;
    QString ts;
};

Track* parseTrack(QJsonValue &data);
QVariantMap parseDownloadInfo(QJsonValue &data);
track_file_download_info parseTrackFileDownloadInfo(QByteArray data);

#endif // TRACKADAPTER_H
