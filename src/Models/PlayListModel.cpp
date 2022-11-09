#include "Models/PlayListModel.h"
#include "Utils/commons.h"

PlayListModel::PlayListModel(QObject *parent) : QAbstractListModel(parent)
{
    _transport = new ApiRequest();
}

int PlayListModel::rowCount(const QModelIndex&) const{
    return _tracks.count();
}

QHash<int,QByteArray>
PlayListModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    //    roles.insert(PlayListModel::ID, "id");
    roles.insert(PlayListModel::TITLE, "title");
    roles.insert(PlayListModel::COVER_URL, "coverUrl");

    return roles;
}


QVariant
PlayListModel::data(const QModelIndex &index, int role) const {
    Q_UNUSED(role)
    if (!index.isValid())
        return QVariant();

    int id = QMetaType::type("Track");
    if (id == QMetaType::UnknownType)
        return QVariant();

    Track* track = _tracks.at(index.row()).value<Track*>();
    switch (role) {
    //        case PlayListModel::ID:
    //            return track.id;
    //            break;
    case PlayListModel::TITLE:
        return track->title;
        break;
    case PlayListModel::COVER_URL:
        return track->coverUrl;
        break;
    }
    return QVariant();
}

//QString
//PlayListModel::makeUrl(const char *format, ...)
//{
//    QString url(MUSIC_API_URL);

//    va_list args;
//    va_start(args, format);

//    QString url2 = QString().vsprintf(format, args);

//    url.append(url2);

//    va_end(args);

//    return url;
//}

//void
//PlayListModel::sendTracksRequest()
//{
//    qDebug() << "NOT IMPLEMENTED";

//}

void
PlayListModel::getTracks()
{

}

int
PlayListModel::getCurrentTrackId()
{
    return _currentTrack->id.toInt();
}

int
PlayListModel::getCurrentAlbumId()
{
    return _currentTrack->album->id.toInt();
}

void
PlayListModel::updateCurrentTrackPlayedSeconds(qint64 ms)
{
    _currentTrack->totalPlayedMs = ms;
}

void
PlayListModel::handleTracksResponse(QJsonValue& data)
{
    QString dataStr = data.toString();
    qint64 dataHash = hash(dataStr);

    if (dataHash == _previousTracksValue) {
        return;
    } else {
        _previousTracksValue = dataHash;
    }

    QJsonObject qjo = data.toObject();
    QJsonArray tracks = qjo["sequence"].toArray();
    _batchId = qjo["batchId"].toString();
    // beginInsertRows(QModelIndex(), m_playList.count(), m_playList.count()+tracks.count()-1);

    foreach (const QJsonValue& value, tracks) {
        QJsonObject trackObject = value.toObject()["track"].toObject();
        QJsonObject albumObject = trackObject["albums"].toArray().at(0).toObject();

        Album* album = new Album;
        album->id = QString::number(albumObject.value("id").toInt());
        album->title = albumObject.value("title").toString();
        album->coverUrl = "https://" + albumObject.value("coverUri").toString();

        Track* newTrack = new Track;
        newTrack->id = trackObject["id"].toString();
        newTrack->title = trackObject["title"].toString();
        newTrack->album = album;

        newTrack->coverUrl = "https://" + trackObject["coverUri"].toString();
//        newTrack->cacheTrackCover("1000x1000");

        for (QJsonValue&& artistObject : trackObject["artists"].toArray()) {
            Artist* artist = new Artist;
            artist->id = QString::number(artistObject.toObject()["id"].toInt());
            artist->name = artistObject.toObject()["name"].toString();
            artist->avatarUrl = "https://" + artistObject.toObject()["cover"].toObject()["uri"].toString();
            newTrack->artists.append(QVariant::fromValue(artist));
        }

        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        _tracks.append(QVariant::fromValue(newTrack));
        endInsertRows();

    }

    // weird
    if (tracks.count() != 0) {
        qDebug() << "Total tracks " << rowCount();
//        qDebug() << "Current index is " << _currentIndex << ". Changing to -1...";
        if (rowCount() == 5)
            _currentIndex = -1;
        else
            _currentIndex = rowCount() - tracks.count();
        qDebug() << "Index now " << _currentIndex;
        emit tracksReceived();
    }
}


QVariant
PlayListModel::get(int idx)
{
    if (idx >= _tracks.count() || idx < 0) {
        return QVariant();
    }

    QMap<QString, QVariant> itemData;

    Track *track = _tracks.at(idx).value<Track*>();

    itemData.insert("title", track->title);
    itemData.insert("coverUrl", track->coverUrl);

    return itemData;
}

void
PlayListModel::setIndex(int idx)
{
    if (idx >= 0 && idx < _tracks.count()) {
        qDebug() << "Set _currentIndex to " << idx;
        _currentIndex = idx;
        _currentTrack = _tracks.at(idx).value<Track*>();
        emit currentTrackUpdated();
    }
}

void
PlayListModel::prepareCurrentTrackToPlay()
{
    _currentTrack->getTrackDownloadInfo();

    connect(_currentTrack, &Track::downloadInfoReady, this, &PlayListModel::trackDownloadInfoReady, Qt::UniqueConnection);
}

void
PlayListModel::trackDownloadInfoReady()
{
    _currentTrack->getTrackFileDownloadLink(_currentTrack->getMaxBitrateAvailable());

    qDebug() << "Requesting track link for bitrate: " << _currentTrack->getMaxBitrateAvailable();

    connect(_currentTrack, &Track::downloadLinkReady, this, &PlayListModel::currentTrackLinkReady, Qt::UniqueConnection);
}

