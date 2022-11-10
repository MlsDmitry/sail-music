#ifndef YACLIENTADAPTER_H
#define YACLIENTADAPTER_H

#include "json_dto/pub.hpp"

#include <string>

#include <QDateTime>
#include <QObject>
#include <QString>

struct play_audio_request_t
{
    play_audio_request_t()
    {
    }

    play_audio_request_t(std::int64_t track_id, std::int64_t album_id, std::string from, std::string from_cache,
                         std::string play_id, std::string uid, std::string timestamp, std::int64_t track_length_seconds,
                         std::int64_t track_played_seconds, std::int64_t end_position_seconds, std::string playlist_id,
                         std::string client_now, std::int64_t timeout)
    {
        _track_id = track_id;
        _album_id = album_id;
        _from = std::move(from);
        _from_cache = std::move(from_cache);
        _play_id = std::move(play_id);
        _uid = std::move(uid);
        _timestamp = std::move(timestamp);
        _track_length_seconds = track_length_seconds;
        _track_played_seconds = track_played_seconds;
        _end_position_seconds = end_position_seconds;
        _playlist_id = std::move(playlist_id);
        _client_now = std::move(client_now);
        _timeout = timeout;
    }

    std::int64_t _track_id;
    std::int64_t _album_id;
    std::string _from;
    std::string _from_cache{""};
    std::string _play_id{""};
    std::string _uid{""};
    std::string _timestamp{""};
    std::int64_t _track_length_seconds{0};
    std::int64_t _track_played_seconds{0};
    std::int64_t _end_position_seconds{0};
    std::string _playlist_id{""};
    std::string _client_now{""};
    std::int64_t _timeout{0};

    template <typename Json_Io> void json_io(Json_Io &io)
    {
        io &json_dto::mandatory("track-id", _track_id) & json_dto::mandatory("album-id", _album_id) &
            json_dto::mandatory("from", _from) & json_dto::optional_no_default("from-cache", _from_cache) &
            json_dto::optional_no_default("play-id", _play_id) & json_dto::optional_no_default("uid", _uid) &
            json_dto::optional_no_default("timestamp", _timestamp) &
            json_dto::optional_no_default("track-length-seconds", _track_length_seconds) &
            json_dto::optional_no_default("total-played-seconds", _track_played_seconds) &
            json_dto::optional_no_default("end-position-seconds", _end_position_seconds) &
            json_dto::optional_no_default("playlist-id", _playlist_id) &
            json_dto::optional_no_default("client-now", _client_now) &
            json_dto::optional_no_default("timeout", _timeout);
    }
};

struct play_audio_request_info
{
    std::int64_t track_id;
    std::int64_t album_id;
    QString from{"mobile-home-rup_main-user-onyourwave-default"};
    QString from_cache{""};
    QString play_id{""};
    QString uid{""};
    QString timestamp{QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ")};
    std::int64_t track_length_seconds{0};
    std::int64_t track_played_seconds{0};
    std::int64_t end_position_seconds{0};
    QString playlist_id{""};
    QString client_now{QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ")};
    std::int64_t timeout{0};
};

QString createPlayAudioRequestData(play_audio_request_info &info);

#endif // YACLIENTADAPTER_H
