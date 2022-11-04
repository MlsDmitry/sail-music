#include "PlayListModel.h"

PlayListModel::PlayListModel(QObject *parent) : QAbstractListModel(parent)
{

}

int PlayListModel::rowCount(const QModelIndex&) const{
    return _musicUnits.count();
}

QHash<int,QByteArray>
PlayListModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles.insert(PlayListModel::ID, "id");
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

    Track* track = _musicUnits.at(index.row()).value<Track*>();

    switch (role) {
        case PlayListModel::ID:
//            return track.id;
            break;
        case PlayListModel::TITLE:
            return track->title;
            break;
        case PlayListModel::COVER_URL:
            return track->coverUrl;
            break;
    }
    return QVariant();
}

