#include "Radio/RadioService.h"
#include "Utils/commons.h"
#include "Utils/Core.h"

#include <QNetworkRequest>
#include <QUrlQuery>
#include <QUrl>
#include <QString>

RadioService::RadioService(QObject *parent) : PlaylistService(parent)
{

}

void
RadioService::requestTracks(QString station)
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
