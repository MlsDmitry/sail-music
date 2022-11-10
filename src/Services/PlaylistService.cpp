#include "PlaylistService.h"
#include "Models/TrackModel.h"
#include "Utils/ApiRequest.h"

PlaylistService::PlaylistService(QObject *parent) : QObject(parent)
{
    _transport = new ApiRequest;
}

void PlaylistService::requestTracks()
{
}
