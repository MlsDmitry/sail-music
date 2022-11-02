#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>

#include "Core.h"

Core::Core(QObject *parent) : QObject(parent)
{
    _transport = new QNetworkAccessManager();

    QNetworkProxy proxy(QNetworkProxy::HttpProxy, "192.168.1.48", 8081);

    _transport->setProxy(proxy);

    connect(_transport, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(replySSLErrors(QNetworkReply*,QList<QSslError>)));


}

void Core::replySSLErrors(QNetworkReply *reply, QList<QSslError> errors)
{
    QList<QSslError> ignoreErrors;
    foreach(QSslError error, errors)
    {
//        qDebug() << QLatin1String("Ignoring SSL error: ")
//                   << (int)error.error()
//                   << QLatin1String(":")
//                   << error.errorString();

        switch( error.error() )
        {
        case QSslError::CertificateUntrusted:
        case QSslError::SelfSignedCertificate:
        case QSslError::SelfSignedCertificateInChain:
            // TODO - remove HostNameMistchmach
            ignoreErrors << error;
            break;
        default:
            ignoreErrors << error;
            break;
        }
    }
    reply->ignoreSslErrors(ignoreErrors);

}
