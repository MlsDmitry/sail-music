#ifndef CORE_H
#define CORE_H

#include <QByteArray>
#include <QJSEngine>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>
#include <QSslError>

#include "Utils/Settings.h"

#define MUSIC_API_URL "https://api.music.yandex.net"

class Core : public QObject
{
    Q_OBJECT
  public:
    Q_PROPERTY(QNetworkAccessManager *transport MEMBER _transport)
    explicit Core(QObject *parent = nullptr);

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new Core();
    }

  private:
    QNetworkAccessManager *_transport;
};

#endif // CORE_H
