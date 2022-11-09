#ifndef YACLIENTSERVICE_H
#define YACLIENTSERVICE_H

#include <QObject>

class YaClientService : public QObject
{
    Q_OBJECT
public:
    explicit YaClientService(QObject *parent = nullptr);

signals:

};

#endif // YACLIENTSERVICE_H
