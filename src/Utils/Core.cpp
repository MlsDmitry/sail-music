#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>

#include "Core.h"

Core::Core(QObject *parent) : QObject(parent)
{
    _transport = new QNetworkAccessManager();

    //    QNetworkProxy proxy(QNetworkProxy::HttpProxy, "192.168.1.48", 8081);

    //    _transport->setProxy(proxy);

    //    connect(_transport, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
    //                this, SLOT(replySSLErrors(QNetworkReply*,QList<QSslError>)));
}
