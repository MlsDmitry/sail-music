#include "Models/PlayListModel.h"
#include "Utils/commons.h"

PlayListModel::PlayListModel(QObject *parent) : QAbstractListModel(parent)
{
    _transport = new ApiRequest();
}

int PlayListModel::rowCount(const QModelIndex &) const
{
    return _tracks.count();
}

QHash<int, QByteArray> PlayListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles.insert(PlayListModel::ID, "id");
    roles.insert(PlayListModel::TITLE, "title");
    roles.insert(PlayListModel::COVER_URL, "coverUrl");

    return roles;
}

QVariant PlayListModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    if (!index.isValid())
        return QVariant();


    Track *track = _tracks.at(index.row()).value<Track *>();
    switch (role)
    {
    case PlayListModel::ID:
        return track->id;
    case PlayListModel::TITLE:
        return track->title;
        break;
    case PlayListModel::COVER_URL:
        return track->coverUrl;
        break;
    }
    return QVariant();
}

void PlayListModel::getTracks()
{
}

int PlayListModel::getCurrentTrackId()
{
    return _currentTrack->id.toInt();
}

int PlayListModel::getCurrentAlbumId()
{
    return _currentTrack->album->id.toInt();
}

void PlayListModel::updateCurrentTrackPlayedSeconds(qint64 ms)
{
    if (ms > 0)
        _currentTrack->totalPlayedMs = ms;
}

QVariant PlayListModel::get(int idx)
{
    if (idx >= _tracks.count() || idx < 0)
    {
        return QVariant();
    }

    QMap<QString, QVariant> itemData;

    Track *track = _tracks.at(idx).value<Track *>();

    itemData.insert("id", track->id);
    itemData.insert("title", track->title);
    itemData.insert("coverUrl", track->coverUrl);

    return itemData;
}

void PlayListModel::setIndex(int idx)
{
    if (idx >= 0 && idx < _tracks.count())
    {
        qDebug() << "Set _currentIndex to " << idx;
        _currentIndex = idx;
        _currentTrack = _tracks.at(idx).value<Track *>();
        emit currentTrackUpdated();
    }
}

void PlayListModel::prepareCurrentTrackToPlay()
{
    _currentTrack->getTrackDownloadInfo();

    connect(_currentTrack, &Track::downloadInfoReady, this, &PlayListModel::trackDownloadInfoReady,
            Qt::UniqueConnection);
}

void PlayListModel::trackDownloadInfoReady()
{
    _currentTrack->getTrackFileDownloadLink(_currentTrack->getMaxBitrateAvailable());

    qDebug() << "Requesting track link for bitrate: " << _currentTrack->getMaxBitrateAvailable();

    connect(_currentTrack, &Track::downloadLinkReady, this, [=](QString url) {
//        _currentTrack->state |= Track::READY_TO_PLAY;
//        emit dataChanged(index(_currentIndex), index(_currentIndex), { ID, TITLE, COVER_URL });
    },
            Qt::UniqueConnection);
}
