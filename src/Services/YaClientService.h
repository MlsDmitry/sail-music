#ifndef YACLIENTSERVICE_H
#define YACLIENTSERVICE_H

#include "Utils/ApiRequest.h"

#include <QObject>

#define PLAY_AUDIO "/play-audio"

class YaClientService : public QObject
{
    Q_OBJECT
  public:
    explicit YaClientService(QObject *parent = nullptr);

    void requestPlayAudio(QString requestData);

  signals:

  private:
    ApiRequest *_transport;
};

#endif // YACLIENTSERVICE_H
