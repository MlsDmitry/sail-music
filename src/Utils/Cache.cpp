#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QNetworkRequest>
#include <QUrl>

#include "Cache.h"

Cache::Cache(QObject *parent) : QObject(parent)
{
    QString imagePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/images";

    QDir cacheDir(imagePath);

    if (!cacheDir.exists())
    {
        cacheDir.mkpath(imagePath);
    }

    //    connect(this, &Cache::cacheImage, this, &Cache::cacheImageByUrl);
}

// void
// Cache::cacheImage(QString urlStr)
//{
//    QUrl url(urlStr);
//    QNetworkRequest request(url);

//    _transport->thirdPartyGetRequest(request, false);

//    connect(_transport, &ApiRequest::thirdPartyDataReady, this, &Cache::saveImageData);
//}

void Cache::saveImageData(QByteArray url, QByteArray data)
{
    QString urlHash = QString(QCryptographicHash::hash(url, QCryptographicHash::Md5).toHex());
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/images/" + urlHash;

    QFile file(path);

    file.open(QFile::ReadWrite);

    file.write(data);

    file.close();

    emit imageSaved(path, urlHash);
}
