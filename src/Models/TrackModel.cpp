#include <QJsonArray>
#include <QJsonObject>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QCryptographicHash>
#include <QThread>

#include "units/Track.h"

Track::Track(QObject *parent) : QObject(parent)
{
    _transport = new ApiRequest();
    _cache = new Cache;
    _cacheThread = new QThread();

    minBitrate = -1; // cause it's quint16 -1 will be the maximum value
    maxBitrate = 0;

    connect(_cache, &Cache::imageSaved, this, &Track::trackCoverSaved);

    _cache->moveToThread(_cacheThread);
}

void
Track::requestDownloadInfo()
{
    QUrl url("https://api.music.yandex.net/tracks/" + id + "/download-info");
    QNetworkRequest request(url);

    _transport->apiGetRequest(request);

    connect(_transport, &ApiRequest::dataReady, this, &Track::handleDownloadInfoResponse);
}

void
Track::handleDownloadInfoResponse(QJsonValue& data)
{
    if (downloadInfos.count() > 0) {
        emit downloadInfoReady();
        return;
    }

    qDebug() << "[+] Fetching download links.";
    for(QJsonValue&& downloadInfo : data.toArray()) {
        QJsonObject downloadInfoObject = downloadInfo.toObject();

        if (downloadInfoObject.value("codec").toString() != "mp3")
            continue;

        int bitrate = downloadInfoObject.value("bitrateInKbps").toInt();

        if (minBitrate > bitrate)
            minBitrate = bitrate;

        if (maxBitrate < bitrate)
            maxBitrate = bitrate;

        downloadInfos.insert(QString::number(bitrate), downloadInfoObject.value("downloadInfoUrl").toString());

        qDebug() << "\tBitrate: " << QString::number(bitrate);
        qDebug() << "\t link: " << downloadInfoObject.value("downloadInfoUrl").toString();

    }

    qDebug() << "[+] Done fetching download links.";

    emit downloadInfoReady();
}

void
Track::requestFileDownloadInfo(QString bitrate)
{
    if (_lastBitrate == bitrate) {
        emit downloadLinkReady(_downloadDirectLinks.value(bitrate).toString());
        return;
    }
    QString downloadUrl = downloadInfos.value(bitrate, QString()).toString();

    if (downloadUrl.length() == 0)
        return;

    _lastBitrate = bitrate;

    QUrl url(downloadUrl);
    QNetworkRequest request(url);

    _transport->thirdPartyGetRequest(request);

    connect(_transport, &ApiRequest::thirdPartyDataReady, this, &Track::handleFileDownloadInfoResponse);

}

void
Track::handleFileDownloadInfoResponse(QNetworkReply* reply)
{
    QVariantMap downloadInfo;
    QString signature;    Q_INVOKABLE void requestDownloadInfo();
    Q_INVOKABLE void requestFileDownloadInfo(QString bitrate);
    Q_INVOKABLE QString getDirectDownloadLink(QString bitrate);
    void cacheTrackCover(QString imageResolution);
    QByteArray data = reply->readAll();
    QXmlStreamReader reader(data);

    if (reader.readNextStartElement()) {
        QStringRef elName = reader.name();

        if (reader.name() == "download-info") {
            while(reader.readNextStartElement()){
                downloadInfo.insert(reader.name().toString(), reader.readElementText());
            }
        }
    }

    signature = QString(QCryptographicHash::hash((("XGRlBW9FXlekgbPrRHuSiA" + downloadInfo.value("path").toString().mid(1) + downloadInfo.value("s").toString()).toUtf8()), QCryptographicHash::Md5).toHex());

    QString directDownloadLink = "https://" + downloadInfo.value("host").toString() + "/get-mp3/" + signature + "/" + downloadInfo.value("ts").toString() + downloadInfo.value("path").toString();

    _downloadDirectLinks.insert(_lastBitrate, directDownloadLink);

    emit downloadLinkReady(directDownloadLink);
}

QString
Track::getDirectDownloadLink(QString bitrate)
{
    return _downloadDirectLinks.value(bitrate, QString()).toString();
}


void
Track::cacheTrackCover(QString imageResolution)
{
    Q_ASSERT(coverUrl.length() > 0);

    coverUrl.replace("%%", imageResolution);

    QUrl url(coverUrl);
    QNetworkRequest request(url);

    _transport->thirdPartyGetRequest(request, false);

    connect(_transport, &ApiRequest::thirdPartyDataReady, this, &Track::trackCoverDataReady);

}

void
Track::trackCoverDataReady(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    reply->deleteLater();
    _cache->saveImageData(reply->url().toEncoded(QUrl::FullyEncoded), data);
}

void
Track::trackCoverSaved(QString imagePath, QString urlHash)
{
    coverUrl = "file:///" + imagePath;

    qDebug() << "Track " << title << " updated cover URL to " << coverUrl;

    emit coverUrlUpdated();
}
