#include "TrackService.h"
#include "Utils/Core.h"
#include "Utils/commons.h"

#include <QJsonValue>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

TrackService::TrackService(QObject *parent) : QObject(parent)
{
    _transport = new ApiRequest();
}

void TrackService::requestDownloadInfo(QString id)
{
    QUrl url(makeUrl(MUSIC_API_URL, TRACK_DOWNLOAD_INFO, id.toStdString().data()));
    QNetworkRequest request(url);

    _transport->apiGetRequest(request);

    connect(
        _transport, &ApiRequest::dataReady, this, [=](QJsonValue &data) { emit trackDownloadInfoReady(data); },
        Qt::UniqueConnection);
}

void TrackService::requestFileDownloadInfo(QString downloadInfoUrl)
{
    QUrl url(downloadInfoUrl);
    QNetworkRequest request(url);

    _transport->thirdPartyGetRequest(request);

    connect(
        _transport, &ApiRequest::thirdPartyDataReady, this,
        [=](QNetworkReply *reply) {
            QByteArray data = reply->readAll();

            reply->deleteLater();

            emit trackFileDownloadInfoReady(data);
        },
        Qt::UniqueConnection);
}
