#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSslError>

#include "Utils/Settings.h"

class ApiRequest : public QObject
{
    Q_OBJECT
  public:
    explicit ApiRequest(QObject *parent = nullptr);

    void thirdPartyGetRequest(QNetworkRequest &request, bool includeAuthorization = true);
    void thirdPartyPostRequest(QNetworkRequest &request, QByteArray &data, bool includeAuthorization = true);
    void apiGetRequest(QNetworkRequest &request);
    void apiPostRequest(QNetworkRequest &request, QByteArray query);

  public slots:
    void replySSLErrors(QNetworkReply *reply, QList<QSslError> errors);
    void responseReady();
    void thirdPartyResponseReady();

  signals:
    void thirdPartyDataReady(QNetworkReply *reply);
    void dataReady(QJsonValue &value);

  private:
    QNetworkAccessManager *_transport;
    Settings _settings;
};

#endif // APIREQUEST_H
