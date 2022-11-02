#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QQmlEngine>
#include <QJSEngine>
#include <QNetworkReply>
#include <QSslError>

class Core : public QObject
{
    Q_OBJECT
//    Q_DISABLE_COPY(Core)
public:
    Q_PROPERTY(QNetworkAccessManager* transport MEMBER _transport)
    explicit Core(QObject *parent = nullptr);

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new Core();
    }
public slots:
    void replySSLErrors(QNetworkReply *reply, QList<QSslError> errors);


private:
    QNetworkAccessManager* _transport;
};

#endif // CORE_H
