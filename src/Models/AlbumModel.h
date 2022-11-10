#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include <QString>

class Album : public QObject
{
    Q_OBJECT
  public:
    Q_PROPERTY(QString id MEMBER id)
    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(QString coverUrl MEMBER coverUrl)

    explicit Album(QObject *parent = nullptr);

    QString id;
    QString title;
    QString coverUrl;
  signals:
};

#endif // ALBUM_H
