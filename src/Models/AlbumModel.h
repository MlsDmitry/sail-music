#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include <QString>

class Album : public QObject
{
    Q_OBJECT
public:
    explicit Album(QObject *parent = nullptr);

    QString id;
    QString title;
    QString coverUrl;
signals:

};

#endif // ALBUM_H
