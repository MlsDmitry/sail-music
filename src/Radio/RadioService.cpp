#include "Radio/RadioService.h"
#include "Utils/Core.h"
#include "Utils/commons.h"

#include <QNetworkRequest>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

RadioService::RadioService(QObject *parent) : PlaylistService(parent)
{
}

void RadioService::requestTracks(QString station)
{
    QNetworkRequest request;
    QUrlQuery query;
    QUrl url(makeUrl(MUSIC_API_URL, STATION_TRACKS, station.toStdString().data()));

    query.addQueryItem("settings2", "true");

    url.setQuery(query);

    request.setUrl(url);

    this->_transport->apiGetRequest(request);

    connect(this->_transport, &ApiRequest::dataReady, this, &RadioService::tracksReceived, Qt::UniqueConnection);
}

void RadioService::requestFeedback(QString station, QString batchId, QString &feedbackData)
{
    QUrl url(makeUrl(MUSIC_API_URL, STATION_FEEDBACK, station.toStdString().data()));
    QNetworkRequest request;
    QUrlQuery query;

    query.addQueryItem("batch-id", batchId);

    url.setQuery(query);

    request.setUrl(url);

    this->_transport->apiPostRequest(request, feedbackData.toUtf8());
}
