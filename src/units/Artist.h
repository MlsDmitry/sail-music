#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>
#include <QString>

class Artist : public QObject
{
    Q_OBJECT
public:
    explicit Artist(QObject *parent = nullptr);

    QString id;
    QString name;
    QString avatarUrl;
signals:

};

Q_DECLARE_METATYPE(Artist*)

#endif // ARTIST_H
