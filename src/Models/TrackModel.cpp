#include "Models/TrackModel.h"
#include "Services/TrackService.h"
#include "Adapters/TrackAdapter.h"

#include <functional>

#include <QJsonArray>
#include <QJsonObject>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QCryptographicHash>
#include <QThread>
#include <QDebug>


Track::Track(QObject *parent) : QObject(parent)
{
    _service = new TrackService();
    //    _cache = new Cache;
    //    _cacheThread = new QThread();

    //    minBitrate = -1; // cause it's quint16 -1 will be the maximum value
    maxBitrate = 0;

    //    connect(_cache, &Cache::imageSaved, this, &Track::trackCoverSaved);

    //    _cache->moveToThread(_cacheThread);
}

void
Track::getTrackDownloadInfo()
{

    _service->requestDownloadInfo(id);

    connect(_service, &TrackService::trackDownloadInfoReady, this, [=](QJsonValue& data) {
        downloadInfos = parseDownloadInfo(data);
        emit downloadInfoReady();
    }, Qt::UniqueConnection);
}

void
Track::getTrackFileDownloadLink(QString bitrate)
{
    Q_ASSERT(downloadInfos.contains(bitrate));

    _service->requestFileDownloadInfo(downloadInfos.value(bitrate).toString());

    connect(_service, &TrackService::trackFileDownloadInfoReady, this, [=](QByteArray data) {
        track_file_download_info info = parseTrackFileDownloadInfo(data);

        QString signature = QString(
                    QCryptographicHash::hash(
                        (("XGRlBW9FXlekgbPrRHuSiA" + info.path.mid(1) + info.signature)).toUtf8(),
                        QCryptographicHash::Md5
                        ).toHex()
                    );

        QString directDownloadLink = "https://" + info.host + "/get-mp3/" + signature + "/" + info.ts + info.path;


        QVariantMap links;

        _downloadDirectLinks.insert(bitrate, directDownloadLink);

        qDebug() << "Got link! " << directDownloadLink;

        emit downloadLinkReady(directDownloadLink);
    }, Qt::UniqueConnection);
}


QString
Track::getDirectDownloadLink(QString bitrate)
{
    return _downloadDirectLinks.value(bitrate, QString()).toString();
}

QString
Track::getMaxBitrateAvailable()
{
    if (maxBitrate == 0) {
        QList<QString> bitratesStr = downloadInfos.keys();

        for (QString& bitrateStr : bitratesStr) {
            if (bitrateStr.toUInt() > maxBitrate)
                maxBitrate = bitrateStr.toUInt();
        }
    }

    return QString::number(maxBitrate);

}


//void
//Track::cacheTrackCover(QString imageResolution)
//{
//    Q_ASSERT(coverUrl.length() > 0);

//    coverUrl.replace("%%", imageResolution);

//    QUrl url(coverUrl);
//    QNetworkRequest request(url);

//    _transport->thirdPartyGetRequest(request, false);

//    connect(_transport, &ApiRequest::thirdPartyDataReady, this, &Track::trackCoverDataReady);

//}

//void
//Track::trackCoverDataReady(QNetworkReply* reply)
//{
//    QByteArray data = reply->readAll();
//    reply->deleteLater();
//    _cache->saveImageData(reply->url().toEncoded(QUrl::FullyEncoded), data);
//}

//void
//Track::trackCoverSaved(QString imagePath, QString urlHash)
//{
//    coverUrl = "file:///" + imagePath;

//    qDebug() << "Track " << title << " updated cover URL to " << coverUrl;

//    emit coverUrlUpdated();
//}
