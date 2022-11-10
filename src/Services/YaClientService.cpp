#include "YaClientService.h"
#include "Utils/Core.h"
#include "Utils/commons.h"

YaClientService::YaClientService(QObject *parent) : QObject(parent)
{
    _transport = new ApiRequest();
}

void YaClientService::requestPlayAudio(QString requestData)
{
    QUrl url(makeUrl(MUSIC_API_URL, PLAY_AUDIO));
    QNetworkRequest request(url);
    QByteArray data = requestData.toUtf8();

    _transport->apiPostRequest(request, data);
}
