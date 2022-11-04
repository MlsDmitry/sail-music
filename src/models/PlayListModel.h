#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QVariant>

#include "units/Track.h"

class PlayListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PlayListModel(QObject *parent = nullptr);

    enum Roles {
        ID = Qt::UserRole + 1,
        TITLE,
        COVER_URL
    };

    Q_ENUM(Roles)

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

private:
    QVariantList _musicUnits;

};

#endif // PLAYLISTMODEL_H
