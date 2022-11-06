#include "PlaylistService.h"
#include "Utils/ApiRequest.h"
#include "Models/TrackModel.h"

PlaylistService::PlaylistService(QObject *parent) : QObject(parent)
{
    _transport = new ApiRequest;
}

void
PlaylistService::requestTracks()
{

}

//void
//PlaylistService::requestTrackDownloadInfo(Track* track)
//{

//    track->requestDownloadInfo();

//    connect(track, &Track::downloadInfoReady, this, &PlayListModel::trackDownloadInfoReady);
//}

//void
//PlaylistService::trackDownloadInfoReady(Track* track)
//{
//    track->requestFileDownloadInfo(QString::number(track->maxBitrate));

//    connect(track, &Track::downloadLinkReady, this, &PlayListModel::currentTrackLinkReady);
//}

