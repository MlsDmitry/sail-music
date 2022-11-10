#include "YaClientAdapter.h"

QString createPlayAudioRequestData(play_audio_request_info &info)
{
    const play_audio_request_t request_info{info.track_id,
                                            info.album_id,
                                            info.from.toStdString(),
                                            info.from_cache.toStdString(),
                                            info.play_id.toStdString(),
                                            info.uid.toStdString(),
                                            info.timestamp.toStdString(),
                                            info.track_length_seconds,
                                            info.track_played_seconds,
                                            info.end_position_seconds,
                                            info.playlist_id.toStdString(),
                                            info.client_now.toStdString(),
                                            info.timeout};

    return QString::fromStdString(json_dto::to_json<play_audio_request_t>(request_info));
}
