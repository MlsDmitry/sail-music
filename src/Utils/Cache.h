#ifndef CACHE_H
#define CACHE_H

#include <QObject>
#include <QString>
#include <QByteArray>

#include "utils/ApiRequest.h"

class Cache : public QObject
{
    Q_OBJECT
public:
    explicit Cache(QObject *parent = nullptr);

public slots:
    void saveImageData(QByteArray url, QByteArray data);

signals:
    void imageSaved(QString imagePath, QString urlHash);

private:
    ApiRequest* _transport;
};

#endif // CACHE_H
