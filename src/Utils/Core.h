#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QQmlEngine>
#include <QJSEngine>
#include <QNetworkReply>
#include <QSslError>
#include <QByteArray>

#include "utils/Settings.h"

#define MUSIC_API_URL   "https://api.music.yandex.net"

class Core : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QNetworkAccessManager* transport MEMBER _transport)
    explicit Core(QObject *parent = nullptr);

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new Core();
    }

private:
    QNetworkAccessManager* _transport;
};

#endif // CORE_H
