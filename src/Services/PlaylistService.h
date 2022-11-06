#ifndef PLAYLISTSERVICE_H
#define PLAYLISTSERVICE_H

#include <QObject>

class PlaylistService : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistService(QObject *parent = nullptr);

signals:

};

#endif // PLAYLISTSERVICE_H
